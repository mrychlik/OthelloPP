/**
 * @file   main.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 14:11:02 2021
 * 
 * @brief  A simple Othello game driver implementation.
 * 
 * 
 */

#include "BoardTraits.hpp"
#include "MainLoop.hpp"
#include <cstdio>     /* for printf */
#include <cstdlib>    /* for exit */
#include <cstring>    /* for basename */


/* From this point on this is good old-fashioned C */

void usage(char *prog) {
  printf("Usage: %s [OPTIONS]...\n"
	 "where OPTIONS may be one of the following:\n"
	 "  -D, --max_depth=N          - maximum search depth for computer (default: 12)\n"
	 "  -d, --computer_delay=N     - number of seconds to wait between moves (default: 0)\n"
	 "  -W, --max_depth_white=N    - maximum search depth for white (default: 12)\n"
	 "  -B, --max_depth_black=N    - maximum search depth for black (default: 12)\n"
	 "  -w, --human_plays_white    - human plays white (default: NO, computer plays white)\n"
	 "  -b, --human_plays_black    - human plays black (default: NO, computer plays black)\n"
	 "  -n, --num_games=N          - number of games (default: 10)\n"
	 "  -P, --print_big            - print a big board (default: ON)\n"
	 "  -p, --print_small          - print a small board (default: OFF)\n"
	 "  -C, --clear_screen         - clear screen before printing next move (default: OFF)\n"
	 "  -c, --board_width=N        - board width (N=4,6 or 8, default: 8)\n"
	 "  -r, --board_height=N       - board height (N=4,6 or 8, default: 8)\n"
	 "  -h, --help                 - print this message and quit\n"	 
	 , prog);
}


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
  int digit_optind = 0;
  while (1) {
    int this_option_optind = optind ? optind : 1;
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
      {"help",                no_argument,       0,  'h' },
      {0,         0,                 0,  0 }
    };
    c = getopt_long(argc, argv, "d:D:W:B:wbn:PpCc:r:h",
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

    case '0':
    case '1':
    case '2':
      if (digit_optind != 0 && digit_optind != this_option_optind)
	printf("digits occur in two different argv-elements.\n");
      digit_optind = this_option_optind;
      printf("option %c\n", c);
      break;

    case 'W':
      MainLoop::getInstance()
	.setMaxDepth(BoardTraits::WHITE, atoi(optarg));
      break;

    case 'B':
      MainLoop::getInstance()
	.setMaxDepth(BoardTraits::BLACK, atoi(optarg));
      break;

    case 'D':
      MainLoop::getInstance()
	.setMaxDepth(BoardTraits::WHITE, atoi(optarg))
	.setMaxDepth(BoardTraits::BLACK, atoi(optarg));
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
	.setHumanPlayer(BoardTraits::WHITE, true);
      break;

    case 'b':
      MainLoop::getInstance()
	.setHumanPlayer(BoardTraits::BLACK, true);
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

    case 'h':
      usage(basename(argv[0]));
      exit(EXIT_SUCCESS);
      break;

    case ':':
      /* missing option argument */
      fprintf(stderr, "%s: option '-%c' requires an argument\n",
	      argv[0], optopt);

    case '?':
    default:
      /* invalid option */
      usage(basename(argv[0]));
      //printf("?? getopt returned character code 0%o (%c) ??\n", c, c);
      exit(EXIT_FAILURE);
      break;
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

