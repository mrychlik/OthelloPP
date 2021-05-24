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
#include <iomanip>

#include "GameTree.hpp"
#include "Board.hpp"


const int DEFAULT_NUM_GAMES = 10; /**< Number of games to play by default */
const int DEFAULT_MAX_DEPTH = 12; /**< Depth to which examine the tree to compute the best move */
const int DEFAULT_MAX_DEPTH = 0; /**< Amount of delay in sec. after computer move */

static int  max_depth[2] = { DEFAULT_MAX_DEPTH, DEFAULT_MAX_DEPTH } ; /**< Max. depth for minmax play for each player*/
static bool humanPlayer[2] = {false, false}; /**< Which player is human? */
static int num_games = DEFAULT_NUM_GAMES; /**< Number of games to play */
static int computer_delay = DEFAULT_COMPUTER_DELAY;

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
  Board::print_size_big = false;
  Board::clear_screen_before_printing = false;
  
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
  int score[num_games] = {0};
  for(int game = 0; game < num_games; ++game) {
    try {
      score[game] = play(game);
    } catch(std::runtime_error& e) {
      std::cout << "Game # " << game << ": "
		<< e.what() << std::endl;
      return 1;
    }
  }
  std::cout << std::setw(5) << "Game" << std::setw(10) << "Score" << std::endl;
  for(int game = 0; game < num_games; ++game) {
    std::cout << std::setw(5) <<  game << std::setw(10) << score[game] << std::endl;
  }
  return 0;
}


/* From this point on this is good old-fashioned C */

#include <unistd.h>    /* For sleep */
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

int main(int argc, char **argv)
{
  int c;

  while (1) {
    //int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"max_depth",           required_argument, 0,  'D' },
      {"max_depth_white",     required_argument, 0,  'W' },
      {"max_depth_black",     required_argument, 0,  'B' },
      {"num_games",           required_argument, 0,  'n' },      
      {"human_plays_white",   no_argument,       0,  'w' },
      {"human_player_black",  no_argument,       0,  'b' },
      {0,         0,                 0,  0 }
    };
    c = getopt_long(argc, argv, "D:W:B:g:wb",
		    long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 0:
      printf("option %s", long_options[option_index].name);
      if (optarg)
	printf(" with arg %s", optarg);
      printf("\n");
      break;

    case 'W':
      max_depth[Board::WHITE] = atoi(optarg);
      printf("max_depth[WHITE]: %d\n", max_depth[Board::WHITE]);      
      break;
    case 'B':
      max_depth[Board::BLACK] = atoi(optarg);
      printf("max_depth[BLACK]: %d\n", max_depth[Board::WHITE]);      
      break;

    case 'D':
      max_depth[Board::WHITE] = atoi(optarg);
      max_depth[Board::BLACK] = max_depth[Board::WHITE];
      printf("max_depth[COMPUTER]: %d\n", max_depth[0]);      
      break;

    case 'n':
      num_games = atoi(optarg);
      printf("num_games: %d\n", num_games);      
      break;

    case 'w':
      humanPlayer[Board::WHITE] = true;
      printf("WHITE played by Human.\n");
      break;

    case 'b':
      humanPlayer[Board::BLACK] = true;
      printf("BLACK played by Human.\n");
      break;

    case '?':
      break;

    default:
      printf("?? getopt returned character code 0%o ??\n", c);
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }

  return main_loop();
}

