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


#include <memory>
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
   * The type of (smart) pointer to node
   * 
   */
  typedef std::shared_ptr<TreeNode> pointer_type;

  /**
   * The type of children container
   * 
   */
  typedef std::forward_list<ptr_type> children_type;

  /**
   * The type of minimax family of functions, which return
   * both the optimal value as well as the pointer to the best child node.
   * 
   */
  typedef std::pair<value_type, pointer_type> search_result_type;


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
  ~TreeNode() { children_.clear(); }
  TreeNode& operator=(TreeNode&& other);

  /** 
   * Read access to the board of this node
   * 
   * 
   * @return 
   */
  const Board& board() const;

  bool isLeaf() const;

  pointer_type getHumanMove(std::istream& s) const;
  pointer_type getComputerMove(const StaticEvaluatorTable& evaluatorTab, int depth, bool prune) const;
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

  const children_type& children() const { expand(); return children_; }

  search_result_type minmax() const;

  search_result_type alphabeta(const StaticEvaluator& evaluator,
			       int depth,
			       bool prune = false,
			       value_type alpha = MIN_VAL,
			       value_type beta = MAX_VAL) const;


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

  Board board_;			/**< The board */
  mutable children_type children_; /**< The children */

  struct {
    BoardTraits::Player player   : 1;	/**< Player to move  */
    mutable bool isExpanded      : 1; /**< Are the children found? */
    int x                        : 4; /**< x of last placed piece, or -1 */
    int y                        : 4; /**< y of last placed piece, or -1 */
  } bits;
    

  void expand() const;
  void trim() const;
  bool isExpanded() { return bits.isExpanded; }
  void swap(TreeNode& other) noexcept;

  template <typename Compare>
  search_result_type alphabeta_helper(const StaticEvaluator& evaluator,
				      int depth,
				      bool prune,
				      value_type alpha, value_type beta,
				      value_type& changing,
				      const value_type& fixed,
				      value_type worst_val,
				      Compare better) const;
};

#endif	// TREE_NODE_SIMPLE_HPP
