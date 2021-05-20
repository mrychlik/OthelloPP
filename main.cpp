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

#include "Board.hpp"
#include "GameTree.hpp"

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
  TreeNode absTreeRoot(new Board(), std::deque<TreeNode*>(), 1, false, 0); //0 value? idk, should not matter

  TreeNode* bestResponse;
  std::thread treeGenerator;
  int mind = 4, maxd = 6;
  int curTileNum;

  std::cout << absTreeRoot->board << std::endl;

  while(absTreeRoot->board->tileNum() != 64) {
    curTileNum = absTreeRoot->board->tileNum();

    //start tree generation
    treeGenerator = std::thread(expandTree, absTreeRoot, (curTileNum+mind), (curTileNum+maxd)); 

    //get player move, if there is one
    Board* possiblePlayerMove = getPlayerMove(absTreeRoot);

    //update tree values
    updateTreeDesireablility(absTreeRoot, curTileNum); //pass tileNum as update#, no repeats

    //check for early game end
    if (possiblePlayerMove != nullptr) {
      if (!(possiblePlayerMove->anyLegalMoves(false) || possiblePlayerMove->anyLegalMoves(true))) {//no one has a move
        treeLock.lock();
        if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
        std::cout << "No possible moves for either player, ending game" << std::endl;
        if (possiblePlayerMove->score() > 0) std::cout << "White Wins";
        else if (possiblePlayerMove->score() < 0) std::cout << "Black Wins";
        else std::cout << "Tie";
        std::cout << std::endl;
        return 0;
      }
    } else {//player can't move, check for possible cpu moves
      if (!absTreeRoot->board->anyLegalMoves(false)) {//black has no moves, game ends
        std::cout << "No possible moves for either player, ending game" << std::endl;
        if (absTreeRoot->board->score() > 0) std::cout << "White Wins";
        else if (absTreeRoot->board->score() < 0) std::cout << "Black Wins";
        else std::cout << "Tie";
        std::cout << std::endl;
        return 0;
      }
    }

    //get best move + do so
    bestResponse = bestMove(absTreeRoot, possiblePlayerMove);

    //print board state
    std::cout << bestResponse->board << std::endl;

    //stop gen tread
    treeLock.lock();
    if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
    treeLock.unlock();

    //garbage collect
    garbageCollect(absTreeRoot, bestResponse);
    absTreeRoot = bestResponse;
  }
  
  //should be unessisary, the early game end should catch 64 tile game ends, just to be safe thoug
  treeLock.lock();
  if (treeGenerator.joinable()) treeGenerator.join(); //hopefully this catches it finishing between this and previous line
  std::cout << "No possible moves for either player, ending game" << std::endl;
  if (absTreeRoot->board->score() > 0) std::cout << "White Wins";
  else if (absTreeRoot->board->score() < 0) std::cout << "Black Wins";
  else std::cout << "Tie";
  std::cout << std::endl;
  return 0;

}
