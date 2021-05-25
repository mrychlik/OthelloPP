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
#include "StaticEvaluator.hpp"

#include <cinttypes>
#include <forward_list>
#include <utility>
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
  /**
   * The top of the node value range. 
   * 
   */
  static const value_type MAX_VAL = std::numeric_limits<value_type>::max();

  /**
   * The bottom of the node value range.
   * 
   */
  static const value_type MIN_VAL = std::numeric_limits<value_type>::min();

  static bool print_recursively; /**< Print childen of the node */

  /**
   * The type of children container
   * 
   */
  typedef std::forward_list<TreeNode*> children_type;

  TreeNode(Player player = WHITE, const Board& board = Board(), int8_t x = -1, int8_t y = -1);
  TreeNode(const TreeNode& other) = delete;
  ~TreeNode();

  TreeNode& operator=(const TreeNode& other); 
  
  bool isLeaf() const;

  const children_type& children() const;

  TreeNode& getHumanMove(std::istream& s) const;
  TreeNode& getComputerMove(const StaticEvaluator& evaluator, int depth) const;

  int nodeCount(int depth) const;

  /** 
   * @return x-coord of last move
   */
  int x() const { return x_; }

  /**
   * @return y-coord of last move
   */
  int y() const { return y_; }

  /** 
   * @return The player to move.
   */
  Player player() const { return player_; };

  int minmax(const StaticEvaluator& evaluator,
	     int8_t depth,
	     value_type alpha = MIN_VAL,
	     value_type beta = MAX_VAL) const;

  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:


  //// NOTE: const methods that operate on mutable fields

  // Installs a new child node
  void addChild(TreeNode* child) const;

  // Add children of a node
  void expandOneLevel(bool verbose = false) const;

  // Delete all children
  void deleteChildren() const;

  //// END: const methods that operate on mutable fields

  //// NOTE: Mutable fields

  mutable bool isExpanded : 1;	/**< Have the children been added */

  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  mutable children_type children_;

  mutable value_type minMaxVal; /**< Cached value by minmax */
  mutable value_type minMaxDepth; /**< Search depth used by minmax */
  mutable const TreeNode* minMaxChild; /**< Best child found by minmax */
  //// END: Mutable fields

  Player player_ : 1;		/**< Player to move  */

  int x_ : 4;			/**< x of last placed piece, or -1 */
  int y_ : 4;			/**< y of last placed piece, or -1 */
};

#endif
