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
  std::cout << "----------------------------------------------------------------\n"
	    << "THE GAME ENDED.\n"
	    << "----------------------------------------------------------------\n"
	    << root << std::endl;
  if( root.score() > 0) {
    std::cout << "WHITE won!!!\n" << root << std::endl;    
  } else if( root.score() < 0) {
    std::cout << "BLACK won!!!.\n" << root << std::endl;
  } else {
    std::cout << "It's a DRAW!!!\n" << root << std::endl;    
  }
}

int main_loop()
{
  while(true)
    try {
      play();
    } catch(std::runtime_error& e) {
      std::cout << e.what() << std::endl;
      return 0;
    }
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
    int option_index = 0;
    static struct option long_options[] = {
      {"max_depth",     required_argument, 0,  0 },
      {"human_plays_white",   no_argument,       0,  'w' },
      {"human_player_black",  no_argument,       0,  'b' },
      {0,         0,                 0,  0 }
    };
    c = getopt_long(argc, argv, "dw:b:",
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

    case 'd':
      max_depth = atoi(optarg);
      printf("max_depth: %d\n", max_depth);      
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

