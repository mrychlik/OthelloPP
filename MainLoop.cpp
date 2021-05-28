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
#include <numeric>

#include "MainLoop.hpp"
#include "GameTree.hpp"
#include "SimpleStaticEvaluator.hpp"
//#include "CornerStaticEvaluator.hpp"


int  MainLoop::max_depth[2]   = { MainLoop::DEFAULT_MAX_DEPTH, MainLoop::DEFAULT_MAX_DEPTH };
bool MainLoop::humanPlayer[2] = {false, false}; 
int MainLoop::num_games       = DEFAULT_NUM_GAMES; 
int MainLoop::computer_delay  = DEFAULT_COMPUTER_DELAY;

/** 
 * Play a game, return the score.
 * 
 * @param game Game number, 0, 1, ...
 * 
 * @return Score
 */
int MainLoop::play(int game, const StaticEvaluatorTable& evaluatorTab)
{
  TreeNode root;
  
  std::cout << root << std::endl;
  while(!root.isLeaf()) {
    std::cout << static_cast<Board>(root) << std::flush
	      << "----------------------------------------------------------------\n" 
	      << "Game #" << game << ": Player " << ( root.player() == Board::WHITE ? "WHITE" : "BLACK") << "\n"
	      << "----------------------------------------------------------------\n"
	      << std::endl;
    if( humanPlayer[root.player()] ) {
      root = root.getHumanMove(std::cin);
      std::cout << "Human played: " << root.x() << " " << root.y() << std::endl;
    } else {			// not human
      ::sleep(computer_delay);
      root = root.getComputerMove(evaluatorTab, max_depth[root.player()]);
      std::cout << static_cast<Board>(root) << std::flush
		<< "----------------------------------------------------------------\n"
		<< "Game #" << game << ": Computer played: " << root.x() << " " << root.y() << "\n"
		<< "----------------------------------------------------------------\n" 
		<< std::endl;
      if(computer_delay > 0) {
	std::cout << "Waiting " << computer_delay << " seconds..." << std::endl;
	::sleep(computer_delay);
      }
    }
  }
  std::cout <<  root << std::flush
	    << "----------------------------------------------------------------\n"
	    << "Game #" << game << ": THE GAME ENDED.\n"
	    << "----------------------------------------------------------------\n"
	    << std::endl;
  if( root.score() > 0) {
    std::cout << root << std::flush
	      << "WHITE won!!! Score " << root.score()
	      << std::endl;    
  } else if( root.score() < 0) {
    std::cout << root << std::flush
	      << "BLACK won!!! Score " << root.score()
	      << std::endl;
  } else {
    std::cout << root << std::flush
	      << "It's a DRAW!!!\n"
	      << std::endl;    
  }
  return root.score();
}

/** 
 * Plays the game of Othello, possibly taking
 * input from the user.
 *
 * @param evaluatorTab Evaluator table
 * @param ins Input stream
 * @param os Output stream
 * @param logs Log stream
 * 
 * @return Status value
 */
int MainLoop::run(std::istream& ins, std::ostream& os, std::ostream& logs,
			const StaticEvaluatorTable& evaluatorTab)
{
  // Seed random number generator, as sometimes we will make random moves
  std::srand(std::time(nullptr)); // use current time as seed for random generator
  int score[num_games] = {0};
  for(int game = 0; game < num_games; ++game) {
    try {
      score[game] = play(game, evaluatorTab);
    } catch(std::runtime_error& e) {
      os << "Game # " << game << ": "
		<< e.what() << std::endl;
      return EXIT_SUCCESS;
    }
  }
  auto average = static_cast<float>(std::accumulate(score, score + num_games, 0)) / num_games;
  logs << std::setw(5) << "Game" << std::setw(10) << "Score" << std::endl;
  for(int game = 0; game < num_games; ++game) {
    logs << std::setw(5) <<  game << std::setw(10) << score[game] << std::endl;
  }
  logs << "-----------------\n"
	    << "Average: " << std::setprecision(2) << average << std::endl;
  
  return EXIT_SUCCESS;
}

/** 
 * A specialization of main_loop that
 * reads from std::cin and writes to std::cout
 * and logs to std::clog.
 *
 * NOTE: Having it makes it possible not to include iostream
 * in MainLoop.hpp
 * 
 * 
 * @return 
 */
int MainLoop::run(const StaticEvaluatorTable& evaluatorTab)
{
  return MainLoop::run(std::cin, std::cout, std::clog, evaluatorTab);
}

/**
 * Default evaluator.
 * 
 */
static const SimpleStaticEvaluator DEFAULT_EVALUATOR;

/**
 * Default evaluator table, one evaluator for each player.
 * 
 */
const StaticEvaluatorTable& MainLoop::DEFAULT_EVALUATOR_TABLE = {&DEFAULT_EVALUATOR, &DEFAULT_EVALUATOR};

const MainLoop& MainLoop::reportSettings() const
{
  // Report on the values of parameters
  std::cout
    << "WHIE played by " << ( humanPlayer[Board::WHITE] ? "Human" : "Computer")
    << "\nBLACK played by " << ( humanPlayer[Board::BLACK] ? "Human" : "Computer" )
    << "\nNumber of games to play: " << num_games
    << "\nMax depth for WHITE: " << max_depth[Board::WHITE]
    << "\nMax depth for BLACK: " << max_depth[Board::BLACK]
    << "\nBoard print size: " << ( Board::print_size_big ? "BIG" : "SMALL" )
    << "\nClear screen before printing: " << ( Board::clear_screen_before_printing ? "ON" : "OFF" )
    << "\nBoard width: " <<  static_cast<unsigned>(Board::w())
    << "\nBoard height: " << static_cast<unsigned>(Board::h())
    << std::endl;

  return *this;
}

const MainLoop& MainLoop::getInstance() {
  // The only instance this class will ever have
  static MainLoop mainLoopSingleton;
  return mainLoopSingleton;
}
  
const MainLoop& MainLoop::setMaxDepth(BoardTraits::Player player, int depth) const {
  max_depth[player] = depth;
  return *this;
}

const MainLoop& MainLoop::setHumanPlayer(BoardTraits::Player player, bool isHuman) const {
  humanPlayer[player] = isHuman;
  return *this;
}

const MainLoop& MainLoop::setComputerDelay(int delay) const {
  computer_delay = delay;
  return *this;
}

const MainLoop& MainLoop::setNumGames(int numGames) const {
  num_games = numGames;
  return *this;
}

const MainLoop& MainLoop::setPrintSizeBig(bool printBig) const {
  Board::print_size_big = printBig;
  return *this;
}

const MainLoop& MainLoop::setClearScreenBbeforePrinting(bool clear) const {
  Board::clear_screen_before_printing = clear;
  return *this;
}

const MainLoop& MainLoop::setBoardWidth(int width) const {
  Board::setW(width);
  return *this;
}

const MainLoop& MainLoop::setBoardHeight(int width) const {
  Board::setH(width);
  return *this;
}



