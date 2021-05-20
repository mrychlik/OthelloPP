/**
 * @file   GameTree.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:34:21 2021
 * 
 * @brief  Game tree implementation
 * 
 * 
 */

#include "GameTree.hpp"


void TreeNode::expand(int minDepth, int maxDepth) {
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
void TreeNode::updateTreeDesireablility(unsigned char upNum) {
  updateNumber = upNum;
  signed char curVal;
  bool initialized = false;
  if (generatedChildren && !downlinks.empty()) {
    for (TreeNode* a : downlinks) {
      if (a->updateNumber != upNum) updateTreeDesireablility(a,upNum); //avoid recalcing when there are loops
      if (board->whitesTurn()) {
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
    value = curVal;
  } else {//no children
    value = board->value();
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
Board* TreeNode::getPlayerMove() {
  if (!board->anyLegalMoves(true)) { //moves for white
    throw "No moves for white.";
  }
  int x,y;

  while(std::cin) {
    std::cout << "Player Move x and y" << std::endl;
    std::cin >> x >> y ; 
    if(std::cin.fail()) {
      std::cerr << "Bad x\n"; 
      continue;
    }
  };

  if(std::cin.bad()) {
    throw "Input stream bad during input of x and y.";
  }
    
  if (x < 0 || x > 7 || y < 0 || y > 7) {
    std::cerr << "Input value x or y is invalid: " << x << ", " << y << "\n";
    throw "Invalid input";
  }
    
  if (!board->isLegal(true, x , y)) {//no good
    std::cerr << "Move is invalid: " << x << ", " << y << "\n";
    throw "Invalid move";
  }
  return board->move(true, x, y);
}

/** 
 * TODO: Something should go here.
 * must be called after update and after getPlayerMove
 * imporart that root be the Node *before* the player's move. Enables catching no player moves
 * should work as long as there are *any* legal moves, for white or black
 * 
 * 
 * @param possiblePlayerMove 
 * 
 * @return 
 */
TreeNode* TreeNode::bestMove(Board* possiblePlayerMove) {
  if (possiblePlayerMove != nullptr) {
    //return most desirable grandchild of player child
    for (TreeNode* a : downlinks) {
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
    for (TreeNode* a : downlinks) { 
      if (a->value == value) { //best of options
        return a;
      }
    }
    return nullptr; //same as above, dummy
  }
}
