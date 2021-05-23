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


/** 
 * @return 
 */
int main() {
  TreeNode absTreeRoot;
  Player player = Board::WHITE;

  std::cout << absTreeRoot << std::endl;

  while(!absTreeRoot.isLeaf()) {
    absTreeRoot.minmax(player);

    //get player move, if there is one
    auto possiblePlayerMove = getPlayerMove(std::cin, absTreeRoot.moves());

    //update tree values
    absTreeRoot.updateTreeDesireablility(curTileNum); //pass numTiles as update#, no repeats
    
    //check for early game end
    auto board = std::get<2>(possiblePlayerMove);
    auto moves = board.moves();

    if ( moves.empty() ) {
      //board.switchPlayer();
      auto moves = board.moves();
      if( moves.empty()) {
	std::cout << "No possible moves for either player, ending game" << std::endl;
	if (board.score() > 0) {
	  std::cout << "White Wins";
	} else if(board.score() < 0) {
	  std::cout << "Black Wins";
	} else {
	  std::cout << "Tie";
	}
	std::cout << std::endl;
	return 0;
      }
    }
  
    //get best move + do so
    auto bestResponse = absTreeRoot.bestMove(possiblePlayerMove);

    //print board state
    std::cout << *bestResponse << std::endl;

    absTreeRoot = *bestResponse;
  }
  
  //if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
  std::cout << "No possible moves for either player, ending game" << std::endl;
  if (absTreeRoot.score() > 0) std::cout << "White Wins";
  else if (absTreeRoot.score() < 0) std::cout << "Black Wins";
  else std::cout << "Tie";
  std::cout << std::endl;
  return 0;

}
