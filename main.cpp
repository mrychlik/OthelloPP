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


const   bool humanPlaysWhite = true;
const   bool humanPlaysBlack = false;
const int MAX_DEPTH =  5;

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

  if(!currentNode.isLeaf()) {
    std::cout << currentNode << std::endl;
    if( isHuman(player) ) {
      auto move = currentNode.getHumanMove();
      currentNode = findChildByMove(move);
    } else {
      currentNode.minmax(MAX_DEPTH);
    }
  } else {
    std::cerr << "The game ended";
  }
  return 0;

}
