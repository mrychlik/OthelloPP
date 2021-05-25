/**
 * @file   main.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 14:11:02 2021
 * 
 * @brief  A simple Othello game driver implementation.
 * 
 * 
 */

#include "Board.hpp"
#include "MainLoop.hpp"
#include <cstdio>     /* for printf */
#include <iostream>

/* From this point on this is good old-fashioned C */

#include <stdlib.h>    /* for exit */
#include <getopt.h>

/** 
 * 
 * 
 * @param argc 
 * @param argv 
 * 
 * @return 
 */
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
      {"computer_delay",      required_argument, 0,  'd' },
      {"print_small",         no_argument,       0,  'p' },
      {"print_big",           no_argument,       0,  'P' },
      {"clear_screen",        no_argument,       0,  'C' },
      {0,         0,                 0,  0 }
    };
    c = getopt_long(argc, argv, "d:D:W:B:g:wbn:PpC",
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
      printf("User set max_depth[WHITE] to %d.\n", max_depth[Board::WHITE]);      
      break;

    case 'B':
      max_depth[Board::BLACK] = atoi(optarg);
      printf("User set max_depth[BLACK] to %d.\n", max_depth[Board::BLACK]);      
      break;

    case 'D':
      max_depth[Board::WHITE] = atoi(optarg);
      max_depth[Board::BLACK] = max_depth[Board::WHITE];
      printf("User set max_depth[(both players)] to %d.\n", max_depth[Board::WHITE]);
      break;

    case 'd':
      computer_delay = atoi(optarg);
      printf("User set computer delay to %d seconds.\n", max_depth[Board::WHITE]);
      break;

    case 'n':
      num_games = atoi(optarg);
      printf("User set num_games to %d.\n",num_games);
      break;

    case 'w':
      humanPlayer[Board::WHITE] = true;
      printf("User set for WHITE to be played by Human.\n");
      break;

    case 'b':
      humanPlayer[Board::BLACK] = true;
      printf("User set for BLACK to be played by Human.\n");
      break;

    case 'C':
      Board::clear_screen_before_printing = true;
      printf("User set clearing screen before printing on.\n");      
      break;

    case 'p':
      Board::print_size_big = false;
      printf("User set for small board printing.\n");
      break;

    case 'P':
      Board::print_size_big = true;
      printf("User set for big board printing.\n");
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
    exit(EXIT_FAILURE);
  }

  // Report on the values of parameters
  printf("max_depth[WHITE]: %d\n", max_depth[Board::WHITE]);      
  if(humanPlayer[Board::BLACK]) printf("BLACK played by Human.\n");
  if(humanPlayer[Board::WHITE]) printf("WHITE played by Human.\n");
  printf("Number of games to play: %d\n", num_games);      
  printf("max_depth[WHITE]: %d\n", max_depth[Board::WHITE]);      
  printf("max_depth[BLACK]: %d\n", max_depth[Board::BLACK]);      
  printf("Board print size: %s\n", Board::print_size_big ? "BIG" : "SMALL");
  printf("Clear screen before printing: %s\n", Board::clear_screen_before_printing ? "ON" : "OFF");

  exit(main_loop(std::cin, std::cout));
}

