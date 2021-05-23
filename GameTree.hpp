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
#include <limits>


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
  static const value_type MAX_VAL = std::numeric_limits<value_type>::max();
  static const value_type MIN_VAL = std::numeric_limits<value_type>::min();

  typedef std::forward_list<TreeNode*> children_type;

  TreeNode(Player player = WHITE, const Board& board = Board());
  //TreeNode(const TreeNode& other) = delete;
  ~TreeNode();

  bool isLeaf() const;
  const children_type& children() const;

  int minmax(Player player, int8_t depth, value_type alpha = MIN_VAL, value_type beta = MAX_VAL);


  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:

  bool isWhitesTurn() const;


  //// NOTE: const methods that operate on mutable fields

  // Installs a new child node
  void addChild(TreeNode* child) const;

  // Add children of a node
  void expandOneLevel(bool verbose = false) const;

  // Delete all children
  void deleteChildren() const;

  //// END: const methods that operate on mutable fields

  //// NOTE: Mutable fields

  mutable bool isExpanded;	/**< Have the children been added */

  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  mutable children_type children_;

  //// END: Mutable fields

  Player player;		/**< Player to move  */

  value_type minmaxValue;	/**< minmax value  */
  uint8_t minmaxDepth;		/**< minmax depth */

  TreeNode *swapPlayer();
};

#endif
