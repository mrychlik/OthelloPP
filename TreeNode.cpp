/**
 * @file   TreeNode.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Thu May 27 21:37:15 2021
 * 
 * @brief  Implements TreeNode factory method
 * 
 * 
 */

#include "TreeNode.hpp"
#include "TreeNodeSimple.hpp"

 TreeNode *TreeNode::create(BoardTraits::Player player, const Board& board, int8_t x, int8_t y)
{
  return new TreeNodeSimple(player, board, x, y);
}
