/**
 * @file   TreeNode.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:34:21 2021
 * 
 * @brief  Game tree implementation
 * 
 * The game Othello is nicely discribed in this YouTube video:
 *     https://www.youtube.com/watch?v=xDnYEOsjZnM
 *
 */

#include "TreeNode.hpp"
#include "BoardTraits.hpp"
#include "StaticEvaluator.hpp"
#include "SimpleStaticEvaluator.hpp"

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
TreeNode::TreeNode(BoardTraits::Player player, const Board& board, int8_t x, int8_t y)
  : board_(board),
    bits({
      .isExpanded = 0,
      .player     = player,
      .x          = x,
      .y          = y,
    })
{      
}

/** 
 * Move constructor.
 * 
 * @param other 
 */
TreeNode::TreeNode(TreeNode&& other)
{
  other.swap(*this);
}

/** 
 * Add child nodes
 *
 * 
 * @return 
 */
void TreeNode::expand() const
{
  if(bits.isExpanded) return;
  auto&& move_bag(moves(player()));
  
  try {
    if( move_bag.empty() ) {	// We have no moves
      // If the other player has a move
      // make it his turn
      if( hasLegalMove(~player()) ) {
	children_.emplace_front(new TreeNode(~player(), board()));
      }
    } else {			// There are moves, we must make one
      while( !move_bag.empty() ) {
	const auto& m  = move_bag.front();
	const auto& [x, y, childBoard ] = m;
	children_.emplace_front(new TreeNode(~player(), childBoard, x, y));
	move_bag.pop_front();
      }
    }
    bits.isExpanded = true;
  } catch(std::bad_alloc& e) {
    std::cerr << e.what() << "\n";
    // Now we have only some children, so not
    // we cannot determine accurate value.
    // NOTE: sets isExpanded to false.
    children_.clear();
    bits.isExpanded = false;
  }
}

/** 
 * Output a TreeNode.
 * 
 * @param s 
 * @param tree 
 * 
 * @return 
 */
std::ostream& TreeNode::print(std::ostream& s) const
{
  return s << board()
    << "\nPlayer to move: " << ( ( player() == Board::WHITE ) ? "WHITE" : "BLACK" )
    << "\nLast filled x: " << x()
    << "\nLast filled y: " << y()
    << std::endl;
}


/** 
 * This alpha-beta algorithm helper function is useful to capture the
 * common logic for both players. The parameters changing and fixed
 * deserve an explanation. These are references to alpha and beta,
 * one a const reference. Depending on whether the player is Max
 * or Min, one of alpha or beta is changing, and the other is fixed.
 * The pay-off is that the helper allows a common implementation of
 *
 * -# plain minmax, when prune is set to false;
 * -#. alpha-abeta pruning when prune is set to true;
 * -#. same code working for Min and Max without performance penalty, which
 *     would result from using conditionals.
 * 
 * @param evaluator 
 * @param depth 
 * @param prune 
 * @param alpha 
 * @param beta 
 * @param changing
 * @param fixed
 * @param worst_val 
 * @param better 
 *
 * @return The minmax value
 */
template <typename Compare>
inline TreeNode::search_result_type
TreeNode::alphabeta_helper(const StaticEvaluator& evaluator,
			   int depth,
			   bool prune,
			   value_type alpha, value_type beta,
			   value_type& changing,
			   const value_type& fixed,
			   value_type worst_val,
			   Compare better) const
{
  value_type bestVal = worst_val;
  std::shared_ptr<TreeNode> bestChild;
  auto children_ = children();
  for( auto& child : children_ ) {
    auto [value, aNode] = child->alphabeta(evaluator, depth - 1, prune, alpha, beta);
    // NOTE: std::max is like f(a,b) -> ( better(a,b) ? b : a ) and better == operator< 
    if( better(bestVal, value) ) {
      bestVal = value;
      bestChild = child;
    }
    if(prune) {
      changing = std::max(changing, bestVal, better);
      if(!better(changing, fixed)) {
	break;
      }
    }
  }
  assert( bestVal != worst_val);
  return search_result_type(bestVal, bestChild);
}


/** 
 * Runs the minmax algorithm with alpha-beta pruning on the tree
 * starting from this node. We note that the alpha-beta pruning
 * algorithm is not guaranteed to find the optimal winning move, only
 * one that produces a better possible value than the opponent's. To
 * produce the highest score, we must perform full minmax with
 * sufficiently high depth.
 *
 * This implementation caches the computed value so that the value of
 * a child can be easily retrieved by calling child's minMaxVal()
 * method.
 *
 * @param evaluator Static evaluator to use
 * @param depth Traverse descendents up to this depth
 * @param prune If true, do pruning, else do not, thus
 *              reverting to mimax.
 * @param alpha Most max can hope for
 * @param beta  Least min can hope for
 * 
 * @return The minmax value
 */
TreeNode::search_result_type
TreeNode::alphabeta(const StaticEvaluator& evaluator, int depth,
		    bool prune,
		    value_type alpha, value_type beta) const
{
  if(depth <= 0 || isLeaf() ) {
    return search_result_type(evaluator(board(), player(), depth), nullptr);
  } 

  // The code could be refactored because Min and Max code is so
  // similar
  if( player() == Board::WHITE ) {	// maximizing player
    return alphabeta_helper(evaluator, depth, prune,
			    alpha, beta,
			    alpha, beta,
			    MIN_VAL,
			    std::less<value_type>());
  } else {			// minimizing player
    assert(player() == Board::BLACK);
    return alphabeta_helper(evaluator, depth, prune,
			    alpha, beta,
			    beta, alpha,
			    MAX_VAL,
			    std::greater<value_type>());
  }
}

/** 
 * Pure minmax algorithm, for reference.
 * 
 * @return MinMax value of the node.
 */
TreeNode::search_result_type
TreeNode::minmax() const
{
  if(isLeaf()) {
    return search_result_type(score(), nullptr);
  } 

  value_type bestVal;
  std::shared_ptr<TreeNode> bestChild;

  auto children_ = children();
  if( player() == Board::WHITE ) {	// maximizing player
    bestVal = MIN_VAL;
    for( auto& child : children_) {
      auto [value, aNode] = child->minmax();
      if(value > bestVal) {
	bestVal = value;
	bestChild = child;
      }
    }
    assert( bestVal != MIN_VAL);
  } else {			// minimizing player
    bestVal = MAX_VAL;
    for( auto& child : children_) {
      auto [value, aNode] = child->minmax();
      if(value < bestVal) {
	bestVal = value;
	bestChild = child;
      }
    }
    assert( bestVal != MAX_VAL);
  }
  return search_result_type(bestVal, bestChild);
}


/** 
 * A leaf node is the final node
 * of the game, i.e. neither player has
 * a valid move.
 * 
 * 
 * @return True if this node is a leaf
 */
bool TreeNode::isLeaf() const
{
  return !hasLegalMove(Board::WHITE) && !hasLegalMove(Board::BLACK);
}

/** 
 * Get the move of the human (interactive) player.
 * We engage in dialog with the user, by prompting to
 * std::cout and reading input from a stream.
 * 
 * 
 * @return The node that user selected.
 */
std::shared_ptr<TreeNode>
TreeNode::getHumanMove(std::istream& s) const
{
  int x,y;
  std::shared_ptr<TreeNode> selectedChild;
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
    if( !(x >= 0 && x < board().w() && y >= 0 && y < board().h()) && !(x == -1 && y == -1) ) {
      std::cout << "Input value x or y out of range: " << x << " " << y << std::endl;
      continue;
    } else {
      auto children_ = children();
      for( auto& child : children_ ) {
	if( child->x() == x && child->y() == y) {
	  selectedChild = child;
	}
      }
      if(selectedChild == nullptr) {
	std::cout << "Move is invalid: " << x << " " << y << std::endl;
	std::cout << "Valid moves:\n";
	for( const auto& child : children()) {
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
	return selectedChild;
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
    for(auto&  child : children() ) {
      count += child->nodeCount(depth - 1);
    }
  }
  return count;
}

/** 
 * Find the best move for the computer.
 *
 * @param evaluatorTab The table of (2) evaluators, one for each player.
 * @param depth Depth of the search.
 * @param prune If true, use alpha-beta pruning.
 * 
 * @return The best child node.
 */
std::shared_ptr<TreeNode> TreeNode::getComputerMove(const StaticEvaluatorTable& evaluatorTab, int depth, bool prune) const
{
  assert(!isLeaf());

  std::vector<std::shared_ptr<TreeNode>> bestChildren;

  if(depth >= 1) {
    search_result_type bestResult;
    if(prune) {
      bestResult = alphabeta(*evaluatorTab[player()], depth, true);
      bestChildren.push_back(bestResult.second);
    } else if(!prune) {
      if(depth < 128) {
	bestResult = alphabeta(*evaluatorTab[player()], depth, false);
      } else {			// depth >= 128
	bestResult = minmax();
      }
    }
    bestChildren.push_back(bestResult.second);
    assert(!bestChildren.empty());
  } else {			// depth <= 0
    // We are very misinformed here because we don't
    // know which child is promising. We choose a random move

    auto sz = std::distance(children().begin(), children().end());
    assert(sz > 0);
    bestChildren.resize(sz);
    // Every child is best
    std::copy(children().begin(), children().end(), bestChildren.begin());
  }

  std::random_shuffle(bestChildren.begin(), bestChildren.end());    
  return *bestChildren.begin();
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
TreeNode& TreeNode::operator=(TreeNode&& other)
{
  if(this == &other) return *this;
  //deleteChildren();
  other.swap(*this);
  return *this;
}

const Board& TreeNode::board() const
{
  return board_;
}

/** 
 * Delegated to Board.
 * 
 * @param player 
 * 
 * @return 
 */
Board::move_bag_type TreeNode::moves(Board::Player player) const {
  return board().moves(player);
}

/** 
 * Delegated to Board
 * 
 * @param player 
 * 
 * @return 
 */
bool TreeNode::hasLegalMove(Board::Player player) const {
  return board().hasLegalMove(player);
}

/** 
 * Delegated to Board
 * 
 * @param player 
 * 
 * @return 
 */
int TreeNode::score() const {
  return board().score();
}

void TreeNode::swap(TreeNode& other) noexcept
{
  if(this == &other) return;
  std::swap(board_, other.board_);
  std::swap(children_, other.children_);  
  std::swap(bits, other.bits);
}
