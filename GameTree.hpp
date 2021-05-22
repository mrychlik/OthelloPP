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
  TreeNode(const Board& b = Board());
  
  int8_t evaluate(uint8_t depth = 0, bool verbose = false);
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;

  void expandOneLevel(bool verbose = false);

  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:

  // Installs a new child node
  void addChild(TreeNode *child);

  int8_t value;			/**< The value of the node */
  bool isExpanded;		/**< Have the children been added */

  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  std::forward_list<TreeNode*> downlinks;

};

#endif
