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
    value(b.value()),
    isExpanded(false),
    downlinks()
{      
}

/** 
 * Destructor. Deletes a tree node and its children
 * 
 */
TreeNode::~TreeNode()
{
  if(isExpanded) {
    for( auto child : downlinks) {
      delete child;
    }
  }
}

/** 
 * Add child nodes
 *
 * @param verbose If true print board, else '.'.
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
  if( !move_bag.empty() ) {
    for (auto m : move_bag) {
      auto child = std::get<2>(m);
      if(verbose) {
	std::clog << ".";
      }
      addChild(new TreeNode(child));
    }
  } else {
    // We don't change the board pieces,
    // just give turn to the opponent
    TreeNode *child = new TreeNode(*this);
    child->setWhitesTurn(!child->isWhitesTurn());
    addChild(child);
  }
  isExpanded = true;
}


void TreeNode::addChild(TreeNode *child)
{
  downlinks.push_front(child);
}

/** 
 * Finds the value of the tree according to Max-Min.
 * Tree is expanded to specified depth.
 * If depth is reached, or if node has no children
 * the static value of the board is returned.
 * 
 * @param depth   Expand tree to this depth
 * @param verbose  Be verbose if true

 * @return The value of this node
 */
int8_t TreeNode::evaluate(uint8_t depth, bool verbose) {
  if(verbose) {
    std::clog << __func__ << ": Reached depth " << static_cast<int>(depth) << "\n";
  }
  auto bestVal = this->Board::value();
  //TreeNode *bestChild = nullptr;
  if(depth > numTiles()) {
    if(!isExpanded) {
      expandOneLevel(verbose);
    }
    for (auto child : downlinks) {
      auto childVal = child->evaluate(depth);
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
  return value;
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
    if (static_cast<const Board& >(*a) == board) { //branch of the players latest move
      for (auto b : a->downlinks) {
	if (b->value == a->value) { //is the best of said options
	  return b; 
	}
      }
    }
  }
  return nullptr; //dummy return, idk if that matters, should be fine, as long as passed board is legal
}

/** 
 * Output a TreeNode. It descends recursively into the tree.
 * 
 * @param s 
 * @param tree 
 * 
 * @return 
 */
std::ostream& operator<<(std::ostream& s, const TreeNode& tree)
{
  s << static_cast<const Board&>(tree)
    << "Is expanded: " << std::boolalpha << tree.isExpanded
    << "\nValue: " << static_cast<int>(tree.value)
    << std::endl;
  for(auto child : tree.downlinks) {
    s << *child << std::endl;
  }
  return s;
}
