/**
 * @file   main.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 14:11:02 2021
 * 
 * @brief  A simple Othello game driver implementation.
 * 
 * 
 */

#include "MainLoop.hpp"
#include <cstdio>     /* for printf */
#include <cstdlib>    /* for exit */


/* From this point on this is good old-fashioned C */

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
      {"board_width",         required_argument, 0,  'c' },
      {"board_height",        required_argument, 0,  'r' },
      {0,         0,                 0,  0 }
    };
    c = getopt_long(argc, argv, "d:D:W:B:g:wbn:PpCc:r:",
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
      MainLoop::getInstance()
	.setMaxDepth(Board::WHITE, atoi(optarg));
      break;

    case 'B':
      MainLoop::getInstance()
	.setMaxDepth(Board::BLACK, atoi(optarg));
      break;

    case 'D':
      MainLoop::getInstance()
	.setMaxDepth(Board::WHITE, atoi(optarg))
	.setMaxDepth(Board::BLACK, atoi(optarg));
      break;

    case 'd':
      MainLoop::getInstance()
	.setComputerDelay(atoi(optarg));
      break;

    case 'n':
      MainLoop::getInstance()
	.setNumGames(atoi(optarg));
      break;

    case 'w':
      MainLoop::getInstance()
	.setHumanPlayer(Board::WHITE, true);
      break;

    case 'b':
      MainLoop::getInstance()
	.setHumanPlayer(Board::BLACK, true);
      break;

    case 'C':
      MainLoop::getInstance()
	.setClearScreenBbeforePrinting(true);
      break;

    case 'p':
      MainLoop::getInstance()
	.setPrintSizeBig(false);
      break;

    case 'P':
      MainLoop::getInstance()
	.setPrintSizeBig(true);
      break;

    case 'c':
      MainLoop::getInstance()
	.setBoardWidth(atoi(optarg));
      break;

    case 'r':
      MainLoop::getInstance()
	.setBoardHeight(atoi(optarg));
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

  auto status = MainLoop::getInstance()
    .reportSettings()
    .run();

  exit(status);
}

