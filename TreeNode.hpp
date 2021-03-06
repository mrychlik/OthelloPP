/**
 * @file   TreeNode.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:26:10 2021
 * 
 * @brief  Game tree 
 * 
 * Generates nodes in a lazy fashion.
 */

#ifndef TREE_NODE_SIMPLE_HPP
#define TREE_NODE_SIMPLE_HPP 1

//#include "TreeNode.hpp"
#include "Board.hpp"
#include "StaticEvaluator.hpp"


#include <cinttypes>
#include <forward_list>
#include <cassert>

/**
 * Class representing the node of the game tree.
 * This class also serves as a child container, by
 * exposing a const iterator interface.
 * 
 * This implementation derives from class Board,
 * as exactly one instance of the Board is associated with a TreeNode.
 * This introduces a compile-time dependency on the Board class, which
 * is undesirable but it seems harmless at this time.
 * 
 */
class TreeNode : public StaticEvaluatorTraits {
public:
  /**
   * The type of children container
   * 
   */
  typedef std::forward_list<TreeNode*> children_type;

  /**
   * Type of value returned by the static evaluator
   * 
   */
  typedef StaticEvaluatorTraits::value_type value_type;

  static bool print_recursively; /**< Print childen of the node */

  TreeNode(BoardTraits::Player player = BoardTraits::BLACK,
	   const Board& board = Board(),
	   int8_t x = -1,
	   int8_t y = -1);

  TreeNode(TreeNode&& other);
  ~TreeNode();
  TreeNode& operator=(TreeNode&& other);
  TreeNode& operator=(const TreeNode& other);

  /** 
   * Read access to the board of this node
   * 
   * 
   * @return 
   */
  const Board& board() const;

  bool isLeaf() const;

  TreeNode getHumanMove(std::istream& s) const;
  TreeNode getComputerMove(const StaticEvaluatorTable& evaluatorTab, int depth, bool prune) const;
  int nodeCount(int depth) const;


  /** 
   * @return x-coord of last move
   */
  int x() const { return bits.x; }

  /**
   * @return y-coord of last move
   */
  int y() const { return bits.y; }

  /** 
   * @return The player to move.
   */
  BoardTraits::Player player() const { return bits.player; };

  int score() const;
  bool hasLegalMove(Board::Player player) const;
  Board::move_bag_type moves(Board::Player player) const;

  const children_type& children() const;

  void minmax() const;

  void alphabeta(const StaticEvaluator& evaluator,
		 int depth,
		 bool prune = false,
		 value_type alpha = MIN_VAL,
		 value_type beta = MAX_VAL) const;

  /** 
   * Returns the node value according
   * to the last run of minmax or alphabeta.
   * 
   * 
   * @return The optimized node value
   */
  value_type minMaxVal() const {
    return bits.minMaxVal;
  }


  /** 
   * Output this node. Simply calls print method.
   * 
   * @param s 
   * @param tree 
   * 
   * @return 
   */
  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree) {
    return tree.print(s);
  };

private:

  static const int DEFAULT_EXPANSION_DEPTH = 1;	/**< Depth when expanding a node */

  std::ostream& print(std::ostream& s) const;

  //// NOTE: const methods that operate on mutable fields

  // Installs a new child node
  void addChild(TreeNode* child) const;

  // Add children of a node
  void expandOneLevel() const;

  // Is the node expanded?
  bool isExpanded() const {
    return bits.isExpanded;
  }

  // Set isExpanded flag
  void setIsExpanded(bool val) const {
    bits.isExpanded = val;
  }

  // Sets minmax value of this node
  void setMinMaxVal(value_type val) const {
    bits.minMaxVal = val;
  }

  // Expad by several levels
  void expandNode(int numLevels = DEFAULT_EXPANSION_DEPTH) const;

  // Delete all descendents
  void deleteDescendents() const;

  // Delete all descendents except for other
  void deleteDescendentsExceptFor(const TreeNode *other) const;

  //// END: const methods that operate on mutable fields

  //// NOTE: Mutable fields

  Board board_;			/**< The board */

  // NOTE: Using deque for this would use 80 bytes of memory
  // under GCC, vector uses only 24 bytes, forward_list 8 bytes.
  // As Board currently consumes 24 bytes, the TreeNode only 32
  mutable children_type children_;

  struct {
    mutable int minMaxVal : 8;	/**< Cached value by minmax */
    mutable bool isExpanded      : 1;	/**< Have the children been added */
    BoardTraits::Player player   : 1;	/**< Player to move  */
    int x                        : 4; /**< x of last placed piece, or -1 */
    int y                        : 4; /**< y of last placed piece, or -1 */
  } bits;
    

  void swap(TreeNode& other) noexcept;

  template <typename Compare>
  void alphabeta_helper(const StaticEvaluator& evaluator,
				       int depth,
				       bool prune,
				       value_type alpha, value_type beta,
				       value_type& changing,
				       const value_type& fixed,
				       value_type worst_val,
				       Compare better) const;
  
};

#endif	// TREE_NODE_SIMPLE_HPP
