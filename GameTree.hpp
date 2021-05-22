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
  typedef std::forward_list<TreeNode*> children_type;

  TreeNode(const Board& b = Board(), bool Player = WHITE);
  ~TreeNode();
  const children_type& children() const;

  
  int8_t evaluate(Player player, uint8_t depth = 0, bool verbose = false);
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;
  int8_t minmax(Player player, uint8_t depth, int8_t alpha = +100, int8_t beta = -100);


  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:

  // Installs a new child node
  void addChild(TreeNode* child) const;
  // Add children of a node
  void expandOneLevel(Player player, bool verbose = false) const;
  // Delete all children
  void deleteChildren() const;


  bool isLeaf() const;
  bool isWhitesTurn() const;


  int8_t value;			/**< The value of the node */
  mutable bool isExpanded;	/**< Have the children been added */
  Player player;		/**< Is the node reflecting white's move */


  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  mutable children_type children_;

};

#endif
