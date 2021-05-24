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
 * @param x       x-coordinate of the last placed piece or -1
 * @param y       y-coordinate of the last placed piece or -1
 *
 * 
 */
TreeNode::TreeNode(Player player, const Board& board, int8_t x, int8_t y)
  : Board(board),
    isExpanded(false),
    children_(),
    player_(player),
    minmaxValue(board.value()),
    minmaxDepth(0),
    x_(x),
    y_(y)
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
  auto move_bag = moves(player());

  try {
    if( move_bag.empty() ) {	// We have no moves
      // If the other player has a move
      // make it his turn
      if( hasLegalMove(~player()) ) {
	addChild(new TreeNode(~player_, *this));
      }
    } else {			// There are moves, we must make one
      for (auto m : move_bag) {
	auto [x, y, childBoard ] = m;
	if(verbose) {
	  std::clog << ".";
	}
	addChild(new TreeNode(~player_, childBoard, x, y));
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
  s << "Player to move: " << ( ( tree.player() == Board::WHITE ) ? 'W' : 'B' )
    << "\nLast filled x: " << tree.x()
    << "\nLast filled y: " << tree.y()
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
int TreeNode::minmax(int8_t depth, value_type alpha, value_type beta) const
{
  if( depth <= 0 || isLeaf() ) {
    minmaxValue = value();
    minmaxDepth = 0;
    return minmaxValue;
  }

  // The code could be refactored because Min and Max code is so
  // similar
  if( player() == WHITE ) {	// maximizing player
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

/** 
 * Returns a reference to the children (container).
 * 
 * @return 
 */
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
 * 
 * @return Child node after the move
 */
TreeNode& TreeNode::getHumanMove(std::istream& s) const
{
  int x,y;
  TreeNode *selectedChild = nullptr;
  while(std::cin) {
    std::cout << "Human, make your move!!\n"
	      << "(Like this: x  y <ENTER>)\n"
	      << "(... or -1 -1 <ENTER>, if you want me, the Computer, to move...)"
	      << std::endl;
    std::cin >> x >> y ; 
    if(std::cin.fail()) {
      std::cout << "Invalid format of x or y (should be a number 0-7)" << std::endl;
      continue;
    } else if(std::cin.bad()) {
      throw std::runtime_error("Input stream bad during input of x and y.");
    }
    std::cout << "You, Human, entered: " << x << " " << y << std::endl;
    if( !(x >= 0 && x <= 7 && y >= 0 && y <= 7) && !(x == -1 && y == -1) ) {
      std::cout << "Input value x or y out of range 0-7: " << x << " " << y << std::endl;
      continue;
    } else {
      for( auto child : children() ) {
	if( child->x() == x && child->y() == y) {
	  selectedChild = child;
	}
      }
      if(selectedChild == nullptr) {
	std::cout << "Move is invalid: " << x << " " << y << std::endl;
	std::cout << "Valid moves:\n";
	for( auto child : children()) {
	  std::cout << child->x() << " " << child->y() << std::endl; 
	}
	std::cout << "Would you like to try again? (Y/N)" << std::endl;
	char c;
	std::cin >> c;
	if(std::cin.bad() || std::cin.fail()) {
	  throw std::runtime_error("Invalid human response to Y/N query.");
	}
	if(c == 'Y') {
	  continue;
	} else if(c == 'N') {
	  std::cout << "Good bye!" << std::endl;
	  throw std::runtime_error("Human said 'No' to Y/N query.");
	} else {
	  continue;
	}
      } else {			// Found valid move
	return *selectedChild;
      }
    }
  }
  throw std::runtime_error("Failed to get a valid move from human.");
}

/** 
 * Returns the node count of the nodes below and including
 * this node, up to given depth.
 * 
 * @param depth 
 * 
 * @return The node count
 */
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

/** 
 * Find best move for the computer
 * 
 * 
 * @return 
 */
TreeNode& TreeNode::getComputerMove(int depth) const
{
  assert(!isLeaf());

  std::vector<TreeNode *> bestChildren;

  auto bestVal = minmax(depth);
  for(auto child : children()) {
    if(child->minmaxValue == bestVal) {
      bestChildren.push_back(child);
    }
  }
  if(bestChildren.empty()) {
    throw std::logic_error("Asked for computer move when there is none");
  } else {
    std::random_shuffle(bestChildren.begin(), bestChildren.end());    
    return **bestChildren.begin();
  }
}

int TreeNode::x() const {
  return x_;
}

int TreeNode::y() const {
  return y_;
}
