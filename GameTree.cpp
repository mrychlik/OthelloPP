/**
 * @file   GameTree.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:34:21 2021
 * 
 * @brief  Game tree implementation
 * 
 * The game Othello is nicely discribed in this YouTube video:
 *     https://www.youtube.com/watch?v=xDnYEOsjZnM
 */

#include "GameTree.hpp"
#include "Board.hpp"
#include <iostream>


/** 
 * Constructor of a node with a given board.
 * 
 * @param b 
 * 
 * @return 
 */
TreeNode::TreeNode(const Board& b)
  : Board(b),
    isExpanded(false),
    updateNumber(0),
    value(b.value())
{      
}


/** 
 * Expands nodes whose depth falls in the range
 * [minDepth, maxDepth).
 * 
 * @param minDepth 
 * @param maxDepth 
 */
void TreeNode::expand(int minDepth, int maxDepth) {
  std::cerr << "started exploration" << std::endl;

  //return and make this a cleaner generational check, but this will work for now
  std::deque<TreeNode*> genStack = std::deque<TreeNode*>();
  std::deque<TreeNode*> queue    = std::deque<TreeNode*>();
  
  queue.push_back(this);

  while (!queue.empty()) {
    auto current = queue.front();
    queue.pop_front(); //returns nothing, see previous line

    if (current->isExpanded) {
      for (TreeNode* a : current->downlinks) {
        queue.push_back(a);
      }
    } else if (current->numTiles() < maxDepth){ 
      auto move_bag = current->moves();
      if( move_bag.empty() ) {
	move_bag = current->moves(!current->isWhitesTurn()); //check for play agains
      }
      for (auto m: move_bag) {
        TreeNode* matched = nullptr;
        //check for existing nodes in generation
	auto board = std::get<2>(m);
        for (TreeNode* n : genStack) {// HELLA SLOW !!!!!!!!!!! FIX w/ custom PQ I guess
          if( *n == board) {
            matched = n;
            break;
          } else if( n->numTiles() <= current->numTiles() ) {
	    break; //only old ones there
	  }
        }
        if (matched == nullptr) {
          TreeNode out(board);
          current->downlinks.push_front(&out); //possibly change value()
          queue.push_back(&out);
        } else {
          current->downlinks.push_front(matched);
          queue.push_back(matched);
        }
      }
      current->isExpanded = true; //must happen after actual gen
    }
    genStack.push_front(current);

    if (current->numTiles() > minDepth) {
      if (current->numTiles() >= minDepth){
        std::cerr << "finished exploration early" << std::endl;
        return; //main is waiting, drop the queue.
      }
    }
  }
  std::cerr << "finished exploration naturally" << std::endl;
}

/** 
 * Finds the value of the tree according to Max-Min.
 * Tree is expanded to specified depth.
 * If depth is reached, or if node has no children
 * the static value of the board is returned.
 * 
 * @param depth 
 */
void TreeNode::evaluate(uint8_t depth) {
  if(depth == 0) {
    return this->Board::value();
  } else {
    if(!isExpanded) {
      expandOne();
    }
    for (auto a : downlinks) {
      aVal = a->evaluate(depth - 1);
      // White is Max, Black is Min
      if (isWhitesTurn()) {
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
    this->value = curVal;
  } else {			//no children

  }
}


/** 
 * TODO: Something should go here.
 *
 * ... must be called after update and after getPlayerMove
 * imporart that root be the Node *before* the player's move. Enables catching no player moves
 * should work as long as there are *any* legal moves, for white or black
 * 
 * 
 * @param possiblePlayerMove 
 * 
 * @return 
 */
TreeNode*
TreeNode::bestMove(const Board::move_type& possiblePlayerMove) const {
  //return most desirable grandchild of player child
  auto& board = std::get<2>(possiblePlayerMove);
  for (auto a : downlinks) {
    if (*static_cast<Board *>(a) == board) { //branch of the players latest move
      for (auto b : a->downlinks) {
	if (b->value == a->value) { //is the best of said options
	  return b; 
	}
      }
    }
  }
  return nullptr; //dummy return, idk if that matters, should be fine, as long as passed board is legal
}

