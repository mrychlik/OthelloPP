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
#include <cassert>

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
    if( move_bag.empty() ) {	// We have no moves
      // If the other player has a move
      // make it his turn
      if( hasLegalMove(~player) ) {
	addChild(new TreeNode(~player, *this));
      }
    } else {			// There are moves, we must make one
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
  s << "Player to move: " << ( ( tree.player == Board::WHITE ) ? 'W' : 'B' )
    << "\nLast filled x: " << static_cast<int>(tree.x)
    << "\nLast filled y: " << static_cast<int>(tree.y) 
    << "\n" << static_cast<const Board&>(tree)
    << "Is expanded: " << std::boolalpha << tree.isExpanded
    << "\nValue: " << static_cast<int>(tree.value())
    << "\nMinMax value: " << static_cast<int>(tree.minmaxValue)
    << "\nMinMax depth: " << static_cast<int>(tree.minmaxDepth)
    << "\n----------------" << std::endl;
	       if(tree.isExpanded) {
		 for(auto child : tree.children()) {
		   s << *child;
		 }
	       }
	       return s;
}

/** 
 * Runs the minmax algorithm with alpha-beta pruning on the
 * tree starting from this node.
 * 
 * @param depth Traverse descendents up to this depth
 * @param alpha Most max can hope for
 * @param beta  Least min can hope for
 * 
 * @return Minmax value of this node
 */
int TreeNode::minmax(int8_t depth, value_type alpha, value_type beta)
{
  if( depth <= minmaxDepth || isLeaf() ) {
    if( depth <= 0) {
      minmaxValue = value();
      minmaxDepth = 0;
    }
    goto end;
  }

  // The code could be refactored because Min and Max code is so
  // similar
  if( player == WHITE ) {	// maximizing player
    value_type bestVal = MIN_VAL;
    for( auto child : children() ) {
      value_type val = child->minmax(depth - 1, alpha, beta);
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
      value_type val = child->minmax(depth - 1, alpha, beta);
      bestVal = std::min(bestVal, val);
      beta = std::min(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    minmaxValue = bestVal;
  }
  minmaxDepth = depth;

 end:

  assert( minmaxValue != MAX_VAL);
  assert( minmaxValue != MIN_VAL);

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
  // TODO: The code below should work but it does not
  //return !hasLegalMove(WHITE) && !hasLegalMove(BLACK);
  // Below is a slow replacement
  return moves(WHITE).empty() && moves(BLACK).empty();
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

const TreeNode::children_type& TreeNode::children() const
{
  if(!isExpanded) {
    expandOneLevel();
  }
  return children_;
}


/** 
 * Reads player move from a stream.
 * Validates the move.
 * 
 * @param s 
 * @param move_bag
 * 
 * @return Triple (x, y, Board)
 */
Board::move_type TreeNode::getHumanMove(std::istream& s) const
{
  int x,y;

  auto move_bag = moves(player);

  while(std::cin) {
    std::cout << "Player Move x and y" << std::endl;
    std::cin >> x >> y ; 
    if(std::cin.fail()) {
      std::cerr << "Bad x\n"; 
      continue;
    }
  };

  if(std::cin.bad()) {
    throw std::runtime_error("Input stream bad during input of x and y.");
  }
    
  if (x < 0 || x > 7 || y < 0 || y > 7) {
    std::cerr << "Input value x or y is invalid: " << x << ", " << y << "\n";
    throw std::runtime_error("Invalid input");
  }

  for( auto& m : move_bag) {
    auto [x1, y1, board1] = m;
    if( x1 == x && y1 == y) {
      return m;
    }
  }

  std::cerr << "Move is invalid: " << x << ", " << y << "\n";
  throw std::runtime_error("Invalid move");
}

int TreeNode::nodeCount(int depth) const
{
  int count = 1;		// Count this node
  if(depth >= 1) {
    for(auto  child : children() ) {
      count += child->nodeCount(depth - 1);
    }
  }
  return count;
}
