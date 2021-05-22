/**
 * @file   GameTree.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:26:10 2021
 * 
 * @brief  Game tree 
 * 
 * 
 */

#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP 1

#include "Board.hpp"
#include <cinttypes>
#include <forward_list>

enum Player {
  BLACK =  0,
  WHITE =  1,
};

inline Player operator~(Player player) {
  return player == WHITE ? BLACK : WHITE;
}

/**
 * Class TreeNode represents the node of the game tree. In this
 * implementation TreeNode derives from class Board, as exactly one
 * instance of the Board is associated with a TreeNode.  This
 * introduces a compile-time dependency on the Board class, which is
 * undesirable but it seems harmless at this time.
 * 
 */
class TreeNode : public Board {
public:
  TreeNode(const Board& b = Board(), bool Player = WHITE);
  ~TreeNode();
  void deleteChildren();
  
  int8_t evaluate(Player player, uint8_t depth = 0, bool verbose = false);
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;

  void expandOneLevel(Player player, bool verbose = false);

  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:

  // Installs a new child node
  void addChild(TreeNode* child);
  int8_t minmax(uint8_t depth, bool isMaximizingPlayer,
		int8_t alpha, int8_t beta);
  bool isLeaf() const;
  bool isWhitesTurn() const;


  int8_t value;			/**< The value of the node */
  bool isExpanded;		/**< Have the children been added */
  bool whitesTurn;		/**< Is the node reflecting white's move */


  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  std::forward_list<TreeNode*> children;

};

#endif
