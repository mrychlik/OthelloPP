/**
 * @file   MainLoop.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 18:14:26 2021
 * 
 * @brief  Declarations for main loop implementation
 * 
 * 
 */

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include <iosfwd>
#include "StaticEvaluator.hpp"

class MainLoop {
private:

  static const int DEFAULT_NUM_GAMES = 10; /**< Number of games to play by default */
  static const int DEFAULT_MAX_DEPTH = 12; /**< Depth to which examine the tree to compute the best move */
  static const int DEFAULT_COMPUTER_DELAY = 0; /**< Amount of delay in sec. after computer move */

public:
  static int  max_depth[2]; ; /**< Max. depth for minmax play for each player*/
  static bool humanPlayer[2]; /**< Which player is human? */
  static int  num_games;      /**< Number of games to play */
  static int  computer_delay; /**< Number of seconds to wait after computer move */

  static const StaticEvaluatorTable& DEFAULT_EVALUATOR_TABLE;

  static int main_loop(std::istream& ins,
		     std::ostream& os,
		     std::ostream& logs,
		     const StaticEvaluatorTable& evaluatorTab = DEFAULT_EVALUATOR_TABLE);

  static int main_loop(const StaticEvaluatorTable& evaluatorTab = DEFAULT_EVALUATOR_TABLE);

private:

  static int play(int game, const StaticEvaluatorTable& evaluatorTab);
};

#endif /* MAIN_LOOP */
