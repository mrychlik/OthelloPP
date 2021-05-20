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

//should I mark some/all as volatile? return to think about, update and gen work at same time I think
class TreeNode {
public:
  Board board;

  TreeNode(const Board& b, std::deque<TreeNode*> d, unsigned char r, bool g, signed char v)
    : board(b),
      downlinks(d),
      referenceCount(r),
      generatedChildren(g),
      updateNumber(0),
      value(v)
  {      
  }
  void expand(int minDepth, int maxDepth);
  void updateTreeDesireablility(unsigned char upNum);

  Board::move_type getPlayerMove() const;
  TreeNode* bestMove(const Board::move_type& possiblePlayerMove) const;

private:

  std::deque<TreeNode*> downlinks;
  unsigned char referenceCount;
  bool generatedChildren;
  unsigned char updateNumber;
  signed char value; //TDB, need to consider what counts as valuable

};

#endif
