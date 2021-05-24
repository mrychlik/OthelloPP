/**
 * @file   main.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 14:11:02 2021
 * 
 * @brief  A simple Othello game driver implementation.
 * 
 * 
 */

#include <iostream>
#include <string>
#include <cstring>

#include "GameTree.hpp"
#include "Board.hpp"


const int DEFAULT_MAX_DEPTH =  13; /**< Depth to which examine the tree to compute the best move */

static int  max_depth = DEFAULT_MAX_DEPTH; /**< Max. depth for minmax play */
static bool humanPlayer[2] = {true, true}; /**< Which player is human? */

void play()
{
  TreeNode root;
  Board::print_size_big = false;
  Board::clear_screen_before_printing = false;
  
  std::cout << root << std::endl;
  while(!root.isLeaf()) {
    std::cout << "----------------------------------------------------------------\n" 
	      << "Player " << ( root.player() == Board::WHITE ? "WHITE" : "BLACK") << "\n"
	      << "----------------------------------------------------------------\n"
	      << std::endl;
    if( humanPlayer[root.player()] ) {
      root = root.getHumanMove(std::cin);
      std::cout << "Human played: " << root.x() << " " << root.y() << "\n"
		<< root << std::endl;
    } else {			// not human
      root = root.getComputerMove(max_depth);
      std::cout << "----------------------------------------------------------------\n"
		<< "Computer played: " << root.x() << " " << root.y() << "\n"
		<< "----------------------------------------------------------------\n" 
		<< root
		<< std::endl;
    }
  }
  std::cout << "The game ended!!!\n" << root << std::endl;
  if( root.score() > 0) {
    std::cout << "WHITE won!!!\n" << root << std::endl;    
  } else if( root.score() < 0) {
    std::cout << "BLACK won!!!.\n" << root << std::endl;
  } else {
    std::cout << "It's a DRAW!!!\n" << root << std::endl;    
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

