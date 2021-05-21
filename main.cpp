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
 * @return 
 */
int main() {
  //declare+initialize
  TreeNode absTreeRoot;
  bool playWHite = true;
  const int mind = 2, maxd = 4;
  std::cout << absTreeRoot << std::endl;
  while(absTreeRoot.numTiles() != 64) {
    auto curTileNum = absTreeRoot.numTiles();
    
    //start tree generation
    auto minDepth = curTileNum + mind;
    auto maxDepth = curTileNum + maxd;

    absTreeRoot.expand(minDepth, maxDepth);

    //get player move, if there is one
    auto possiblePlayerMove = absTreeRoot.getPlayerMove();

    //update tree values
    absTreeRoot.updateTreeDesireablility(curTileNum); //pass numTiles as update#, no repeats
    
    //check for early game end
    auto board = std::get<2>(possiblePlayerMove);
    auto moves = board.moves();

    if ( moves.empty() ) {
      board.switchPlayer();
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
    } else if ( moves_black.empty() ) {//black has no moves, game ends
      std::cout << "No possible moves for either player, ending game" << std::endl;
      if (absTreeRoot.score() > 0) std::cout << "White Wins";
      else if (absTreeRoot.score() < 0) std::cout << "Black Wins";
      else std::cout << "Tie";
      std::cout << std::endl;
      return 0;
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
