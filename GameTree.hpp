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
 * on the Board class, which is undesirable
 * but it seems harmless at this time.
 * 
 */
class TreeNode : public Board {
public:
  TreeNode(const Board& b = Board());
  
  int8_t evaluate(uint8_t depth = 0);
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;

  void expandOneLevel(bool verbose = false);

  friend std::ostream& operator<<(std::ostream& s, const TreeNode& tree);

private:

  void addChild(TreeNode *child);

  std::vector<TreeNode*> downlinks;
  bool isExpanded;
  uint8_t updateNumber;
  int8_t value; //TDB, need to consider what counts as valuable


};

#endif
