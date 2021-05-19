#include <iostream>
#include <string>
#include <cstring>
#include "Oboard.hh"

#ifndef deque
#include <deque>
#endif

//need Priority Queue of some kind (for intergeneration comparisons)
//#include <priority_queue>
//internally uses heap vector, not sure if that is what I need, can overload internal type though
#include <thread>
#include <mutex>

//must be a global variable, as mtx can't be passed as args. seems like bad practice but eh, I have no choice
std::mutex treeLock; //starts unlocked


//should I mark some/all as volatile? return to think about, update and gen work at same time I think
struct TreeNode {
  Board* board;
  std::deque<TreeNode*> downlinks;
  unsigned char referenceCount;
  bool generatedChildren;
  unsigned char updateNumber;
  signed char value; //TDB, need to consider what counts as valuable
};

TreeNode tn(Board* b, std::deque<TreeNode*> d, unsigned char r, bool g, signed char v) {
  TreeNode out;
  out.board = b;
  out.downlinks = d;
  out.referenceCount = r;
  out.generatedChildren = g;
  out.updateNumber = 0;
  out.value = v;
  return out;
}

void expandTree (TreeNode* root, int minDepth, int maxDepth) {
  std::cerr << "started exploration" << std::endl;
  //return and make this a cleaner generational check, but this will work for now
  std::deque<TreeNode*> genStack = std::deque<TreeNode*>();
  
  std::deque<TreeNode*> queue = std::deque<TreeNode*>();
  queue.push_back(root);
  TreeNode* current;
  while (!queue.empty()) {
    current = queue.front();
    queue.pop_front(); //returns nothing, see previous line

    if (current->generatedChildren) {
      for (TreeNode* a : current->downlinks) {
        queue.push_back(a);
      }
    } else if (current->board->tileNum() < maxDepth){ 
      std::deque<Board*> children = current->board->children(current->board->whitesTurn());
      if (children.empty()) children = current->board->children(!current->board->whitesTurn()); //check for play agains
      TreeNode* matched;
      for (Board* b : children) {
        matched = nullptr;
        //check for existing nodes in generation
        for (TreeNode* n : genStack) {// HELLA SLOW !!!!!!!!!!! FIX w/ custom PQ I guess
          if (*(n->board) == *b) {
            matched = n;
            delete b; //former mem leak. idk about deleting in foreach tho?
            break;
          } else if (n->board->tileNum() <= current->board->tileNum()) break; //only old ones there
        }
        if (matched == nullptr) {
          TreeNode* out = new TreeNode();
          *out = tn(b, std::deque<TreeNode*>(), 1, false, b->value());
          current->downlinks.push_front(out); //possibly change value()
          queue.push_back(out);
        } else {
          current->downlinks.push_front(matched);
          matched->referenceCount++;
          queue.push_back(matched);
        }
      }
      current->generatedChildren = true; //must happen after actual gen
    }
    genStack.push_front(current);

    if (current->board->tileNum() > minDepth) {
      if (treeLock.try_lock()) {
        //succeeds in locking, no sweat. keep working
        treeLock.unlock();
      } else if (current->board->tileNum() >= minDepth){
        std::cerr << "finished exploration early" << std::endl;
        return; //main is waiting, drop the queue.
      }
    }
  }
  std::cerr << "finished exploration naturally" << std::endl;
}

//recursive to update the value of a the whole tree
//hopefully safe to work on tree at same time as gen
void updateTreeDesireablility(TreeNode* n, unsigned char upNum) {
  n->updateNumber = upNum;
  signed char curVal;
  bool initialized = false;
  if (n->generatedChildren && !n->downlinks.empty()) {
    for (TreeNode* a : n->downlinks) {
      if (a->updateNumber != upNum) updateTreeDesireablility(a,upNum); //avoid recalcing when there are loops
      if (n->board->whitesTurn()) {
        //since computer just played, the value of n should be the *greatest* of the downlinks, assume player is smart
        //(postive value is good for white, neg is good for black)
        //each is one possible player move
        if (!initialized || a->value > curVal){
           curVal = a->value;
           initialized = true;
        }
      } else { 
        //player just moved. each dl is a CPU move, should choose min
        //negative is good for black, so we want the lowest possible value of the DLs
        if (!initialized || a->value < curVal) {
          curVal = a->value;
          initialized = true;
        }
      }
    }
    n->value = curVal;
  } else {//no children
    n->value = n->board->value();
  }
}

//must be called as only thread on tree to avoid segfaults
void garbageCollect(TreeNode* oldRoot, TreeNode* newRoot) {  
  std::deque<TreeNode*> toDel = std::deque<TreeNode*>(); //dfs
  toDel.push_front(oldRoot);
  TreeNode* current;
  while (!toDel.empty()) {
    current = toDel.front();
    toDel.pop_front();
    if (--(current->referenceCount) == 0 && current != newRoot) {
      if (current->generatedChildren) {
        for (TreeNode* a : current->downlinks) toDel.push_front(a);
      }
      delete current->board;
      delete current;
    }
  }
}

//return tileNum=0 if no moves available
Board* getPlayerMove(TreeNode* currentRoot) {
  if (currentRoot->board->anyLegalMoves(true)) { //moves for white
    bool goodInput;
    int x,y;

    do {
      goodInput = true;
      try {
        std::cout << "Player Move x" << std::endl;
        std::cin >> x; 
        std::cout << "Player Move y" << std::endl;
        std::cin >> y; 
      } catch (...) {//not numbers, etc
        goodInput = false; 
      }
      
      if (goodInput) {
        if (x < 0 || x > 7 || y < 0 || y > 7) {
          goodInput = false;
        } else if (!currentRoot->board->isLegal(true, x , y)) {//no good
          goodInput = false;
        }
      }
    } while (!goodInput);
    
    // if the input is good
    return currentRoot->board->move(true, x, y);
  } else {
    return nullptr; //empty (tileNum = 0)
  }
}

//must be called after update and after getPlayerMove
//imporart that root be the Node *before* the player's move. Enables catching no player moves
//should work as long as there are *any* legal moves, for white or black
TreeNode* bestMove(TreeNode* root, Board* possiblePlayerMove) {
  if (possiblePlayerMove != nullptr) {
    //return most desirable grandchild of player child
    for (TreeNode* a : root->downlinks) {
      if (*(a->board) == *possiblePlayerMove) { //branch of the players latest move
        for (TreeNode* b : a->downlinks) {
          if (b->value == a->value) { //is the best of said options
            return b; 
          }
        }
      }
    }
    return nullptr; //dummy return, idk if that matters, should be fine, as long as passed board is legal
  } else {
    //pick best child of root
    for (TreeNode* a : root->downlinks) { 
      if (a->value == root->value) { //best of options
        return a;
      }
    }
    return nullptr; //same as above, dummy
  }
}


int main() {
  //declare+initialize
  TreeNode* absTreeRoot = new TreeNode;
  *absTreeRoot = tn(new Board(), std::deque<TreeNode*>(), 1, false, 0); //0 value? idk, should not matter

  TreeNode* bestResponse;
  std::thread treeGenerator;
  int mind = 4, maxd = 6;
  int curTileNum;

  std::cout << absTreeRoot->board->toStringBig() << std::endl;

  while(absTreeRoot->board->tileNum() != 64) {
    curTileNum = absTreeRoot->board->tileNum();

    //start tree generation
    treeGenerator = std::thread(expandTree, absTreeRoot, (curTileNum+mind), (curTileNum+maxd)); 

    //get player move, if there is one
    Board* possiblePlayerMove = getPlayerMove(absTreeRoot);

    //update tree values
    updateTreeDesireablility(absTreeRoot, curTileNum); //pass tileNum as update#, no repeats

    //check for early game end
    if (possiblePlayerMove != nullptr) {
      if (!(possiblePlayerMove->anyLegalMoves(false) || possiblePlayerMove->anyLegalMoves(true))) {//no one has a move
        treeLock.lock();
        if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
        std::cout << "No possible moves for either player, ending game" << std::endl;
        if (possiblePlayerMove->score() > 0) std::cout << "White Wins";
        else if (possiblePlayerMove->score() < 0) std::cout << "Black Wins";
        else std::cout << "Tie";
        std::cout << std::endl;
        return 0;
      }
    } else {//player can't move, check for possible cpu moves
      if (!absTreeRoot->board->anyLegalMoves(false)) {//black has no moves, game ends
        std::cout << "No possible moves for either player, ending game" << std::endl;
        if (absTreeRoot->board->score() > 0) std::cout << "White Wins";
        else if (absTreeRoot->board->score() < 0) std::cout << "Black Wins";
        else std::cout << "Tie";
        std::cout << std::endl;
        return 0;
      }
    }

    //get best move + do so
    bestResponse = bestMove(absTreeRoot, possiblePlayerMove);

    //print board state
    std::cout << bestResponse->board->toStringBig() << std::endl;

    //stop gen tread
    treeLock.lock();
    if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
    treeLock.unlock();

    //garbage collect
    garbageCollect(absTreeRoot, bestResponse);
    absTreeRoot = bestResponse;
  }
  
  //should be unessisary, the early game end should catch 64 tile game ends, just to be safe thoug
  treeLock.lock();
  if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
  std::cout << "No possible moves for either player, ending game" << std::endl;
  if (absTreeRoot->board->score() > 0) std::cout << "White Wins";
  else if (absTreeRoot->board->score() < 0) std::cout << "Black Wins";
  else std::cout << "Tie";
  std::cout << std::endl;
  return 0;

}
