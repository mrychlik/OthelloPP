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

  while(!currentNode.isLeaf()) {
    std::cout << currentNode << std::endl;
    if( isHuman(player) ) {
      auto selectedChild = currentNode.getHumanMove(std::cin);
      std::cout << "Human played: " << selectedChild.x() << " " << selectedChild.y() << "\n"
		<< selectedChild << std::endl;
      currentNode = selectedChild;
    } else {			// not human
      auto selectedChild = currentNode.getComputerMove(MAX_DEPTH);
      std::cout << "Computer played: " << selectedChild.x() << " " << selectedChild.y() << "\n"
		<< board << std::endl;
      currentNode = selectedChild;
    }
  } else {
    std::cerr << "The game ended!!!\n" << currentNode;
  }
  return 0;

}
