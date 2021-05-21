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

//need Priority Queue of some kind (for intergeneration comparisons)
//#include <priority_queue>
//internally uses heap vector, not sure if that is what I need, can overload internal type though
#include <thread>
#include <mutex>

//must be a global variable, as mtx can't be passed as args. seems like bad practice but eh, I have no choice
std::mutex treeLock; //starts unlocked




int main() {
  //declare+initialize
  TreeNode absTreeRoot; //0 value? idk, should not matter

  const int mind = 4, maxd = 6;

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
    auto moves_black = board.moves(false);
    auto moves_white = board.moves(true);

    if ( moves_black.empty() && moves_white.empty()) {//no one has a move
      std::cout << "No possible moves for either player, ending game" << std::endl;
      if (board.score() > 0) std::cout << "White Wins";
      else if (board.score() < 0) std::cout << "Black Wins";
      else std::cout << "Tie";
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
  
  //should be unessisary, the early game end should catch 64 tile game ends, just to be safe thoug
  treeLock.lock();
  //if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
  std::cout << "No possible moves for either player, ending game" << std::endl;
  if (absTreeRoot.score() > 0) std::cout << "White Wins";
  else if (absTreeRoot.score() < 0) std::cout << "Black Wins";
  else std::cout << "Tie";
  std::cout << std::endl;
  return 0;

}
