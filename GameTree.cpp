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
 * Add child nodes
 * 
 * 
 * @return 
 */
void TreeNode::expandOneLevel(bool verbose)
{
  if(isExpanded) {
    return;
  }
  auto move_bag = moves();
  for (auto m : move_bag) {
    auto child = std::get<2>(m);
    if(verbose) std::clog << child;
    addChild(new TreeNode(child));
  }
}


void TreeNode::addChild(TreeNode *child)
{
  downlinks.push_back(child);
}

/** 
 * Finds the value of the tree according to Max-Min.
 * Tree is expanded to specified depth.
 * If depth is reached, or if node has no children
 * the static value of the board is returned.
 * 
 * @param depth 
 */
int8_t TreeNode::evaluate(uint8_t depth) {
  auto bestVal = this->Board::value();
  //TreeNode *bestChild = nullptr;
  if(depth == 0) {
    return bestVal;
  } else {
    if(!isExpanded) {
      expandOneLevel();
    }
    for (auto child : downlinks) {
      auto childVal = child->evaluate(depth - 1);
      // White is Max, Black is Min
      if (isWhitesTurn()) {
	if(childVal > bestVal) {
	  bestVal = childVal;
	  //bestChild = child;
	}
      } else { 
	if(childVal < bestVal) {
	  bestVal = childVal;
	  //bestChild = child;	  
	}
      }
    }
  }
  value = bestVal;
  return bestVal;
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

std::ostream& operator<<(std::ostream& s, const TreeNode& tree)
{
  s << static_cast<const Board&>(tree)
    << "\nIs expanded: " << tree.isExpanded
    << "\nValue: " << static_cast<int>(tree.value)
    << "\nUpdate number: " << static_cast<int>(tree.updateNumber)
    << std::endl;
  for(auto child : tree.downlinks) {
    s << *child << std::endl;
  }
  return s;
}
