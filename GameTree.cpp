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
 * Constructor of a node with a given player and board.
 * 
 * @param player
 * @param board
 *
 * 
 */
TreeNode::TreeNode(Player player, const Board& board, int8_t x, int8_t y)
  : Board(board),
    isExpanded(false),
    children_(),
    player(player),
    minmaxValue(board.value()),
    minmaxDepth(0),
    x(x),
    y(y)
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
    if( move_bag.empty() ) {
      // If the other player has a move
      // make it his turn
      if( hasLegalMove(~player) ) {
	addChild((new TreeNode(*this))->swapPlayer());
      }
    } else {			// There are moves
      for (auto m : move_bag) {
	auto [x, y, childBoard ] = m;
	if(verbose) {
	  std::clog << ".";
	}
	addChild(new TreeNode(~player, childBoard, x, y));
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
    << "Player to move: " << ( ( tree.player == Board::WHITE ) ? 'W' : 'B' )
    << "\nIs expanded: " << std::boolalpha << tree.isExpanded
    << "\nIs white's turn: " << std::boolalpha << tree.isWhitesTurn()
    << "\nValue: " << static_cast<int>(tree.value())
    << "\nMinMax value: " << static_cast<int>(tree.minmaxValue)
    << "\nMinMax depth: " << static_cast<int>(tree.minmaxDepth)
    << "\nLast piece x and y: << " << tree.x << ", " << tree.y
    << std::endl;
  return s;
}

int TreeNode::minmax(Player player, int8_t depth, value_type alpha, value_type beta)
{
  if( depth <= 0 || isLeaf() ) {
    minmaxDepth = 0;
    minmaxValue = value();
    return minmaxValue;
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
    minmaxValue = bestVal;
  } else {			// minimizing player
    value_type bestVal = MAX_VAL;
    for( auto child : children() ) {
      value_type val = child->minmax(WHITE, depth - 1, alpha, beta);
      bestVal = std::min(bestVal, val);
      beta = std::min(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    minmaxValue = bestVal;
  }

  minmaxDepth = depth;
  return minmaxValue;
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

TreeNode *TreeNode::swapPlayer()
{
  deleteChildren();
  minmaxValue = value();
  minmaxDepth = 0;
  player = ~player;
  return this;
}
