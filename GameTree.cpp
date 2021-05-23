/**
 * @file   GameTree.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:34:21 2021
 * 
 * @brief  Game tree implementation
 * 
 * The game Othello is nicely discribed in this YouTube video:
 *     https://www.youtube.com/watch?v=xDnYEOsjZnM
 *
 * This pseudocode is adapted from below 
 * source:
 *
 */

#include "GameTree.hpp"
#include "Board.hpp"
#include <iostream>
#include <algorithm>


/** 
 * Constructor of a node with a board.
 * 
 * @param board
 *
 * 
 */
TreeNode::TreeNode(const Board& board, Player player)
  : Board(board),
    isExpanded(false),
    children_(),
    player(player)
{      
}

/** 
 * Destructor. Deletes a tree node and its children
 * 
 */
TreeNode::~TreeNode()
{
  if(isExpanded) {
    for( auto child : children() ) {
      delete child;
    }
  }
}

/** 
 * Add child nodes
 *
 * @param verbose If true print board, else '.'.
 * 
 * @param player
 * 
 * @return 
 */
void TreeNode::expandOneLevel(bool verbose) const
{
  if(isExpanded) return;
  auto move_bag = moves(player);

  try {
    if( !move_bag.empty() ) {
      for (auto m : move_bag) {
	auto childBoard = std::get<2>(m);
	if(verbose) {
	  std::clog << ".";
	}
	addChild(new TreeNode(childBoard, ~player));
      }
    }
    isExpanded = true;
  } catch(std::bad_alloc& e) {
    std::cerr << e.what() << "\n";
    // Now we have only some children, so not
    // we cannot determine accurate value.
    // NOTE: sets isExpanded to false.
    deleteChildren();
  }
}


void TreeNode::addChild(TreeNode* child) const
{
  children_.push_front(child);
}

/** 
 * Output a TreeNode.
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
    << "\nIs white's turn: " << std::boolalpha << tree.isWhitesTurn()
    << "\nValue: " << static_cast<int>(tree.value())
    << std::endl;
  return s;
}

void TreeNode::minmax(Player player, int8_t depth, value_type alpha, value_type beta)
{
  if( depth < 0 || isLeaf() ) {
    minMaxValue = value();
    return;
  }

  // The code could be refactored because Min and Max code is so
  // similar
  if( player == WHITE ) {	// maximizing player
    value_type bestVal = MIN_VAL;
    for( auto child : children() ) {
      value_type val = child->minmax(BLACK, depth - 1, alpha, beta);
      bestVal = std::max(bestVal, val);
      alpha = std::max(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    return bestVal;
  } else {			// minimizing player
    value_type bestVal = MAX_VAL;
    for( auto child : children() ) {
      value_type val = child->minmax(WHITE, depth - 1, alpha, beta);
      bestVal = std::min(bestVal, val);
      alpha = std::min(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    minMaxValue = bestVal;
  }
}

/** 
 * A leaf node is the final node
 * of the game, i.e. neither player has
 * a valid move.
 * 
 * 
 * @return 
 */
bool TreeNode::isLeaf() const
{
  return !hasLegalMove(WHITE) && !hasLegalMove(BLACK);
}



/** 
 * Deletes all children and marks
 * the node unexpanded
 * 
 */
void TreeNode::deleteChildren() const
{
  for(auto child : children_ ) {
    delete child;
  }
  // Empty the list
  children_.clear();
  isExpanded = false;
}

bool TreeNode::isWhitesTurn() const
{
  return player == WHITE;
}

const TreeNode::children_type& TreeNode::children() const
{
  if(!isExpanded) {
    expandOneLevel();
  }
  return children_;
}
