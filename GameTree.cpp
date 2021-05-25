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

bool TreeNode::print_recursively = false;

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
    minMaxVal(0),
    minMaxDepth(0),
    minMaxChild(this),
    player_(player),
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
  s << static_cast<const Board&>(tree)
    << "\nPlayer to move: " << ( ( tree.player() == Board::WHITE ) ? "WHITE" : "BLACK" )
    << "\nLast filled x: " << tree.x()
    << "\nLast filled y: " << tree.y()
    << std::endl;

  if(tree.isExpanded && TreeNode::print_recursively) {
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
 * We cache these computer values for the node and descendents as
 * minmaxDepth and minmaxValue. However, these values are
 * not valid if the evaluator is changed.
 * 
 * @param evaluator Static evaluator to use
 * @param depth Traverse descendents up to this depth
 * @param alpha Most max can hope for
 * @param beta  Least min can hope for
 * 
 * @return The best child of this node
 */
int TreeNode::minmax(const StaticEvaluator& evaluator, int8_t depth, value_type alpha, value_type beta) const
{
  if(depth <= 0 || isLeaf() ) {
    minMaxChild = this;
    minMaxVal = evaluator(*this, player(), depth);
  } else if(minMaxDepth == depth) { // depth > 0
    assert(minMaxChild == this);
    return minMaxVal;
  }

  // The code could be refactored because Min and Max code is so
  // similar
  if( player() == WHITE ) {	// maximizing player
    value_type bestVal = MIN_VAL;
    TreeNode* bestChild = nullptr;
    for( auto child : children() ) {
      value_type val = child->minmax(evaluator, depth - 1, alpha, beta);
      if(val > bestVal) {
	bestVal = val;
	bestChild = child;
      }
      alpha = std::max(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    minMaxVal = bestVal;
    minMaxChild = bestChild;
  } else {			// minimizing player
    value_type bestVal = MAX_VAL;
    TreeNode* bestChild = nullptr;
    for( auto child : children() ) {
      value_type val = child->minmax(evaluator, depth - 1, alpha, beta);
      if(val > bestVal) {
	bestVal = val;
	bestChild = child;
      }
      beta = std::min(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    minMaxVal = bestVal;
    minMaxChild = bestChild;
  }
  assert( minMaxVal != MAX_VAL);
  assert( minMaxVal != MIN_VAL);
  assert( minMaxChild != nullptr);
  minMaxDepth = depth;
  return minMaxVal;
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
	std::cout << "Would you like to try again? (say: Y/N/Q)" << std::endl;
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
	} else if(c == 'Q') {
	  throw std::runtime_error("Human quit");
	} else {
	  std::cout << "Try again, Human!" << std::endl;
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
TreeNode& TreeNode::getComputerMove(const StaticEvaluator& evaluator, int depth) const
{
  assert(!isLeaf());
  if(depth >= 1) {
    std::vector<TreeNode *> bestChildren;

    auto bestVal = minmax(evaluator, depth);
    for(auto child : children()) {
      if(child->minMaxVal == bestVal) {
	bestChildren.push_back(child);
      }
    }
    assert(!bestChildren.empty());
    std::random_shuffle(bestChildren.begin(), bestChildren.end());    
    return **bestChildren.begin();
  } else {			// depth <= 0
    // We are very misinformed here because we don't
    // know which child is promising. We choose a random move

    auto sz = std::distance(children().begin(), children().end());
    std::vector<TreeNode *> bestChildren(sz);
    // Every child is best
    std::copy(children().begin(), children().end(), bestChildren.begin());

    // Reshufle children first
    std::random_shuffle(bestChildren.begin(), bestChildren.end());    
    return **bestChildren.begin();
  }
}


/** 
 * This copy assignment notably allows a copy from
 * a child of the current node. In this case,
 * we delete all children but the one from which
 * we copy. We take ownership of all child's
 * successors, empty its children container
 * and set child's status to unexpanded.
 * 
 * @param other 
 * 
 * @return 
 */
TreeNode& TreeNode::operator=(const TreeNode& other)
{
  if(this == &other) return *this;
  // Delete children other than other;
  // Since children_ is mutable, nothing
  // bad should happen
  bool isOtherChild = false;
  for(auto child : children_) {
    if(child != &other) {
      delete child;
    } else {
      isOtherChild = true;
    }
  }
  if(!isOtherChild) {
    throw std::logic_error("We can only copy-assign childen");
  }
  children_.clear();
  isExpanded = false;
  
  if(other.isExpanded) {
    children_ = other.children_;
    // Now that we took other's children,
    // we need to inform other
    // that he has none
    other.children_.clear();
    other.isExpanded = false;
    isExpanded = true;
  }

  // Copy base class object
  *static_cast<Board*>(this) = *static_cast<const Board*>(&other);
  // Copy normal fields
  player_ = other.player_;

  x_ = other.x_;
  y_ = other.y_;

  return *this;
}
