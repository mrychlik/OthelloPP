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

#include <deque>
#include "Board.hpp"

/**
 * Class TreeNode represents the node of
 * the game tree. In this implementation
 * TreeNode derives from class Board,
 * as exactly one instance of the Board
 * is associated with a TreeNode.
 * This introduces a compile-time dependency
 * on the Board class.
 * 
 */
class TreeNode : public Board {
public:
  /** 
   * Constructor of a node with a given board.
   * 
   * @param b 
   * 
   * @return 
   */
  TreeNode(const Board& b = Board())
    : Board(b),
      isExpanded(false),
      updateNumber(0),
      value(b.value())
  {      
  }
  
  /** 
   * Expands nodes whose depth falls in the range
   * [minDepth, maxDepth).
   * 
   * @param minDepth 
   * @param maxDepth 
   */
  void expand(int minDepth, int maxDepth);
  void updateTreeDesireablility(unsigned char upNum);

  Board::move_type getPlayerMove() const;
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;

private:

  std::deque<TreeNode*> downlinks;
  bool isExpanded;
  unsigned char updateNumber;
  signed char value; //TDB, need to consider what counts as valuable
};

#endif
