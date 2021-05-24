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
  TreeNode root(player);

  while(!root.isLeaf()) {
    std::cout << root << std::endl;
    if( isHuman(player) ) {
      const TreeNode& selectedChild = root.getHumanMove(std::cin);
      std::cout << "Human played: " << selectedChild.x() << " " << selectedChild.y() << "\n"
		<< selectedChild << std::endl;
      root = selectedChild;
    } else {			// not human
      const TreeNode& selectedChild = root.getComputerMove(MAX_DEPTH);
      std::cout << "Computer played: " << selectedChild.x() << " " << selectedChild.y() << "\n"
		<< selectedChild << std::endl;
      root = selectedChild;
    }
  }
  std::cerr << "The game ended!!!\n" << root;
  return 0;

}
