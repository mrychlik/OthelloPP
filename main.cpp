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
const   bool humanPlaysBlack = true; /**< Is human playing black? */

const int MAX_DEPTH =  13;	/**< Depth to which examine the tree to compute the best move */

static bool isHuman(Board::Player player) {
  if(player == Board::WHITE && humanPlaysWhite) {
    return true;
  } else if(player == Board::BLACK && humanPlaysBlack) {
    return true;
  } else {
    return false;
  }
};

void play() {
  TreeNode root;
  Board::print_size_big = false;
  Board::clear_screen_before_printing = false;
  
  std::cout << root << std::endl;
  while(!root.isLeaf()) {
    std::cout << "----------------------------------------------------------------\n" 
	      << "Player " << ( root.player() == Board::WHITE ? "WHITE" : "BLACK") << "\n"
	      << "----------------------------------------------------------------\n"
	      << std::endl;
    if( isHuman(root.player()) ) {
      root = root.getHumanMove(std::cin);
      std::cout << "Human played: " << root.x() << " " << root.y() << "\n"
		<< root << std::endl;
    } else {			// not human
      root = root.getComputerMove(MAX_DEPTH);
      std::cout << "----------------------------------------------------------------\n"
		<< "Computer played: " << root.x() << " " << root.y() << "\n"
		<< "----------------------------------------------------------------\n" 
		<< root
		<< std::endl;
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
}

#include <unistd.h>		// For sleep

int main()
{
  while(true)
    try {
      play();
    } catch(std::runtime_error& e) {
      std::cout << e.what();
      return 0;
    }
  }
}
