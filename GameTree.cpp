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
 * Constructor of a node with a given board.
 * 
 * @param board
 *
 * 
 * @return 
 */
TreeNode::TreeNode(const Board& board, bool whitesTurn)
  : Board(board),
    value(board.value()),
    isExpanded(false),
    whitesTurn(whitesTurn),
    children()
{      
}

/** 
 * Destructor. Deletes a tree node and its children
 * 
 */
TreeNode::~TreeNode()
{
  if(isExpanded) {
    for( auto child : children) {
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
void TreeNode::expandOneLevel(Player player, bool verbose)
{
  if(isExpanded) {
    if( player == WHITE )  {    
      return;
    } else {			// We expanded for the other player
      deleteChildren();
    }
  }
  auto move_bag = moves(player);

  try {
    if( !move_bag.empty() ) {
      for (auto m : move_bag) {
	auto child = std::get<2>(m);
	if(verbose) {
	  std::clog << ".";
	}
	addChild(new TreeNode(child, player));
      }
    } else if(!isLeaf(){
	// We don't change the board pieces,
	// just give turn to the opponent
	addChild(new TreeNode(*this, ~player));
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


void TreeNode::addChild(TreeNode* child)
{
  children.push_front(child);
}

/** 
 * Finds the value of the tree according to Max-Min.
 * Tree is expanded to specified depth.
 * If depth is reached, or if node has no children
 * the static value of the board is returned.
 * 
 * @param player
 * @param depth   Expand tree to this depth
 * @param verbose  Be verbose if true

 * @return The value of this node
 */
int8_t TreeNode::evaluate(Player player, uint8_t depth, bool verbose) {
  if(verbose) {
    std::clog << __func__ << ": Depth " << static_cast<int>(depth)
	      << ", Number of tiles: " <<  static_cast<int>(numTiles())
	      << ", Is leaf: " << std::boolalpha << isLeaf()
	      << std::endl;
  }
  auto bestVal = this->Board::value();
  if( !isLeaf() && (depth > numTiles()) ) {
    if(!isExpanded) {
      expandOneLevel(player, verbose);
    }
    // Now the node may not have expanded
    // because of memory allocation failure
    if(isExpanded) {
      for (auto child : children) {
	auto childVal = child->evaluate(~player, depth, verbose);
	// White is Max, Black is Min
	if (player == WHITE) {
	  bestVal = std::max(bestVal, childVal);
	} else { 
	  bestVal = std::min(bestVal, childVal);
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
  for (auto a : children) {
    if (static_cast<const Board& >(*a) == board) { //branch of the players latest move
      for (auto b : a->children) {
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
    << "\nIs white's turn: " << std::boolalpha << tree.isWhitesTurn()
    << "\nValue: " << static_cast<int>(tree.value)
    << std::endl;
  for(auto child : tree.children) {
    s << *child << std::endl;
  }
  return s;
}

int8_t TreeNode::minmax(uint8_t depth, bool isMaximizingPlayer,
			int8_t alpha, int8_t beta)
{
  if( isLeaf() ) return value;

  // The code could be refactored because Min and Max code is so
  // similar
  if( isMaximizingPlayer ) {	// maximizing player
    int8_t bestVal = -100;
    for( auto child : children) {
      auto val = child->minmax(depth + 1, false, alpha, beta);
      bestVal = std::max(bestVal, val);
      alpha = std::max(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    return bestVal;
  } else {			// minimizing player
    int8_t bestVal = +100;
    for( auto child : children) {
      auto val = child->minmax(depth + 1, false, alpha, beta);
      bestVal = std::min(bestVal, val);
      alpha = std::min(alpha, bestVal);
      if( beta <= alpha) {
	break;
      }
    }
    return bestVal;
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
  return hasLegalMove(WHITE) && !hasLegalMove(BLACK);
}



/** 
 * Deletes all children and marks
 * the node unexpanded
 * 
 */
void TreeNode::deleteChildren()
{
  for(auto child : children) {
    delete child;
  }
  // Empty the list
  children.clear();
  isExpanded = false;
}

bool TreeNode::isWhitesTurn() const
{
  return whitesTurn;
}
