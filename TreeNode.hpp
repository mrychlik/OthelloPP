/**
 * @file   TreeNode.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 18:26:10 2021
 * 
 * @brief  Abstract game tree 
 * 
 * Provides abstract base class TreeNode for all game tree node
 * implementation classes.
 */

#ifndef ABSTRACT_TREE_NODE_HPP
#define ABSTRACT_TREE_NODE_HPP 1

#include "Board.hpp"
#include "StaticEvaluator.hpp"

#include <iosfwd>
#include <cinttypes>

/**
 * Class representing the node of the game tree.
 * This class also serves as a child container, by
 * exposing a const iterator interface.
 * 
 */
class TreeNode : public StaticEvaluatorTraits, public BoardTraits {
public:
  /**
   * The type of children container
   * 
   */
  typedef std::forward_list<TreeNode*> children_type;

  /** 
   * Implements TreeNode factory function.
   * 
   * @param player 
   * @param board 
   * @param x 
   * @param y 
   * 
   * @return 
   */
  static TreeNode *create(BoardTraits::Player player = WHITE,
			  const Board& board = Board(),
			  int8_t x = -1,
			  int8_t y = -1);
  virtual ~TreeNode() {};

  /** 
   * Read access to the board of this node
   * 
   * 
   * @return 
   */
  virtual const Board& board() const = 0;

  /** 
   * Yields the static score of this node.
   * Must be equivalent to 
   *   return this->board().score;
   * 
   * @return 
   */
  virtual int score() const = 0;

  /** 
   * This copy assignment allows copy assignmet from
   * a child of the current node and disallows
   * copying from unrelated nodes.
   * 
   * @param other 
   * 
   * @return 
   */
  virtual TreeNode& operator=(const TreeNode& other) = 0; 

  /** 
   * A leaf node is the final node
   * of the game, i.e. neither player has
   * a valid move.
   * 
   * 
   * @return True if this node is a leaf
   */
  virtual bool isLeaf() const = 0;

  

  /** 
   * Reads player move from a stream.
   * Validates the move.
   * 
   * @param s 
   * 
   * @return Child node after the move
   */
  virtual TreeNode& getHumanMove(std::istream& s) const = 0;
  
  /** 
   * Find the best move for the computer.
   * 
   * 
   * @return The best child node.
   */
  virtual TreeNode& getComputerMove(const StaticEvaluatorTable& evaluatorTab, int depth) const = 0;

  virtual int nodeCount(int depth) const = 0;
  /** 
   * @return x-coord of last move
   */
  virtual int x() const = 0;

  /**
   * @return y-coord of last move
   */
  virtual int y() const = 0;

  /** 
   * @return The player to move.
   */
  virtual Player player() const  = 0;

  /**
   * Read only access to child container
   *
   * @return Children.
   */
  virtual const children_type& children() const = 0;

  /** 
   * Runs the minmax algorithm with alpha-beta pruning on the
   * tree starting from this node. 
   *
   * @param evaluator Static evaluator to use
   * @param depth Traverse descendents up to this depth
   * @param alpha Most max can hope for
   * @param beta Least min can hope for
   * 
   * @return The best child of this node
   */
  virtual int minmax(const StaticEvaluator& evaluator,
	     int8_t depth,
	     value_type alpha = MIN_VAL,
	     value_type beta = MAX_VAL) const = 0;

  virtual std::ostream& print(std::ostream& s) const = 0;

  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree) {
    return tree.print(s);
  };
};

#endif	// ABSTRACT_TREE_NODE_HPP
