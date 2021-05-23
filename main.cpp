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


#ifndef deque
#include <deque>
#endif

/** 
 * Reads player move from a stream.
 * Validates the move against moves.
 * 
 * @param s 
 * @param move_bag
 * 
 * @return Triple (x, y, Board)
 */
Board::move_type
getPlayerMove(std::istream& s, const Board::move_bag_type& move_bag)
{
  int x,y;

  while(std::cin) {
    std::cout << "Player Move x and y" << std::endl;
    std::cin >> x >> y ; 
    if(std::cin.fail()) {
      std::cerr << "Bad x\n"; 
      continue;
    }
  };

  if(std::cin.bad()) {
    throw std::runtime_error("Input stream bad during input of x and y.");
  }
    
  if (x < 0 || x > 7 || y < 0 || y > 7) {
    std::cerr << "Input value x or y is invalid: " << x << ", " << y << "\n";
    throw std::runtime_error("Invalid input");
  }

  for( auto& m : move_bag) {
    auto [x1, y1, board1] = m;
    if( x1 == x && y1 == y) {
      return m;
    }
  }

  std::cerr << "Move is invalid: " << x << ", " << y << "\n";
  throw std::runtime_error("Invalid move");
}

const int MAX_DEPTH =  7;

/** 
 * @return 
 */
int main() {
  Board::Player player = Board::WHITE;
  TreeNode absTreeRoot(player);

  TreeNode& currentNode(absTreeRoot);

  if(!currentNode.isLeaf()) {
    std::cout << currentNode << std::endl;
    currentNode.minmax(player, MAX_DEPTH);
  }
  return 0;

}
