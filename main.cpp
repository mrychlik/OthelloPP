/**
 * @file   main.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 14:11:02 2021
 * 
 * @brief  Othello game driver implementation.
 * 
 * 
 */

#include <iostream>
#include <string>
#include <cstring>

#include "GameTree.hpp"
#include "Board.hpp"


const   bool humanPlaysWhite = true; /**< Is human playing white? */
const   bool humanPlaysBlack = false; /**< Is human playing black? */

const int MAX_DEPTH =  5;	/**< Depth to which examine the tree to compute the best move */

static bool isHuman(Board::Player player) {
  if(player == Board::WHITE && humanPlaysWhite) {
    return true;
  } else if(player == Board::WHITE && humanPlaysWhite) {
    return true;
  } else {
    return false;
  }
};

/** 
 * @return 
 */
int main() {
  Board::Player player = Board::WHITE;
  TreeNode absTreeRoot(player);
  TreeNode& currentNode(absTreeRoot);

  if(!currentNode.isLeaf()) {
    std::cout << currentNode << std::endl;
    if( isHuman(player) ) {
      auto move = currentNode.getHumanMove(std::cin);
      auto [x, y, board] = move;
      std::cout << "Human played: " << x << " " << y
		<< board << std::endl;
    } else {
      currentNode.minmax(MAX_DEPTH);
    }
  } else {
    std::cerr << "The game ended";
  }
  return 0;

}
