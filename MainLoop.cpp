/**
 * @file   MainLoop.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 18:13:42 2021
 * 
 * @brief  Implements the main loop of the Othello game
 * 
 * 
 */
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <iomanip>

#include "GameTree.hpp"



const int DEFAULT_NUM_GAMES = 10; /**< Number of games to play by default */
const int DEFAULT_MAX_DEPTH = 12; /**< Depth to which examine the tree to compute the best move */
const int DEFAULT_COMPUTER_DELAY = 0; /**< Amount of delay in sec. after computer move */

int  max_depth[2] = { DEFAULT_MAX_DEPTH, DEFAULT_MAX_DEPTH } ; /**< Max. depth for minmax play for each player*/
bool humanPlayer[2] = {false, false}; /**< Which player is human? */
int num_games = DEFAULT_NUM_GAMES; /**< Number of games to play */
int computer_delay = DEFAULT_COMPUTER_DELAY; /**< Number of seconds to wait after computer move */

/** 
 * Play a game, return the score.
 * 
 * @param game Game number, 0, 1, ...
 * 
 * @return Score
 */
int play(int game)
{
  TreeNode root;
  
  std::cout << root << std::endl;
  while(!root.isLeaf()) {
    std::cout << "----------------------------------------------------------------\n" 
	      << "Game #" << game << ": Player " << ( root.player() == Board::WHITE ? "WHITE" : "BLACK") << "\n"
	      << "----------------------------------------------------------------\n"
	      << std::endl;
    if( humanPlayer[root.player()] ) {
      root = root.getHumanMove(std::cin);
      std::cout << "Human played: " << root.x() << " " << root.y() << "\n"
		<< root << std::endl;
    } else {			// not human
      root = root.getComputerMove(max_depth[root.player()]);
      std::cout << "----------------------------------------------------------------\n"
		<< "Game #" << game << ": Computer played: " << root.x() << " " << root.y() << "\n"
		<< "----------------------------------------------------------------\n" 
		<< root
		<< std::endl;
      if(computer_delay > 0) {
	std::cout << "Waiting " << computer_delay << " seconds..." << std::endl;
	::sleep(computer_delay);
      }
    }
  }
  std::cout << "----------------------------------------------------------------\n"
	    << "Game #" << game << ": THE GAME ENDED.\n"
	    << "----------------------------------------------------------------\n"
	    << root << std::endl;
  if( root.score() > 0) {
    std::cout << "WHITE won!!! Score " << root.score() << "\n"
	      << root << std::endl;    
  } else if( root.score() < 0) {
    std::cout << "BLACK won!!! Score " << root.score() << "\n"
	      << root << std::endl;    
  } else {
    std::cout << "It's a DRAW!!!\n" << root << std::endl;    
  }
  return root.score();
}

int main_loop()
{
  // Seed random number generator, as sometimes we will make random moves
  std::srand(std::time(nullptr)); // use current time as seed for random generator
  int score[num_games] = {0};
  for(int game = 0; game < num_games; ++game) {
    try {
      score[game] = play(game);
    } catch(std::runtime_error& e) {
      std::cout << "Game # " << game << ": "
		<< e.what() << std::endl;
      return EXIT_SUCCESS;
    }
  }
  std::cout << std::setw(5) << "Game" << std::setw(10) << "Score" << std::endl;
  for(int game = 0; game < num_games; ++game) {
    std::cout << std::setw(5) <<  game << std::setw(10) << score[game] << std::endl;
  }
  return EXIT_SUCCESS;
}

