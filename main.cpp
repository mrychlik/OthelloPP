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


const   bool humanPlaysWhite = false; /**< Is human playing white? */
const   bool humanPlaysBlack = false; /**< Is human playing black? */

const int MAX_DEPTH =  13;	/**< Depth to which examine the tree to compute the best move */

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
  TreeNode root;
  
  while(!root.isLeaf()) {
    if( isHuman(root.player()) ) {
      std::cout << static_cast<Board>(root) << std::endl;
      root = root.getHumanMove(std::cin);
      std::cout << "Human played: " << root.x() << " " << root.y() << "\n"
		<< static_cast<Board>(root) << std::endl;
    } else {			// not human
      root = root.getComputerMove(MAX_DEPTH);
      std::cout << "Computer played: " << root.x() << " " << root.y() << "\n"
		<< static_cast<Board>(root) << std::endl;
    }
  }
  std::cout << "The game ended!!!\n" << root << std::endl;
  if( root.score() > 0) {
    std::cout << "WHITE won.\n" << root << std::endl;    
  } else if( root.score() < 0) {
    std::cout << "BLACK won.\n" << root << std::endl;
  } else {
    std::cout << "IT'S A DRAW!.\n" << root << std::endl;    
  }
  return 0;

}
