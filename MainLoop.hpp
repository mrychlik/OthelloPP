/**
 * @file   MainLoop.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 18:14:26 2021
 * 
 * @brief  Declarations for main loop implementation
 * 
 */

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include <iosfwd>
#include <memory>
#include "StaticEvaluator.hpp"

/**
 * This class runs the game loop and controls 
 * numerous game settings. It provides "fluid" style interface for ease of use,
 * e.g.:
 *
 * MainLoop::getInstance()
 *   ->setMaxDepth(Board::WHITE, 12)
 *   ->setMaxDepth(Board::BLACK, 12)  
 *   ->setNumGames(1)
 *   ->run();
*/
class MainLoop {
private:

  static const int DEFAULT_NUM_GAMES = 10; /**< Number of games to play by default */
  static const int DEFAULT_MAX_DEPTH = 12; /**< Depth to which examine the tree to compute the best move */
  static const int DEFAULT_COMPUTER_DELAY = 0; /**< Amount of delay in sec. after computer move */

public:

  static const MainLoop* getInstance() {
    // The only instance this class will ever have
    static MainLoop mainLoopSingleton;
    return &mainLoopSingleton;
  }
  
  /** 
   * Set game depth for a player.
   * 
   * @param player 
   * @param depth 
   * 
   * @return this
   */
  auto setMaxDepth(Board::Player player, int depth) const {
    max_depth[player] = depth;
    return this;
  }

  /** 
   * Designates player to be human, or otherwise
   * capable of interactive play.
   * 
   * @param player 
   * @param isHuman 
   * 
   * @return this
   */
  auto setHumanPlayer(Board::Player player, bool isHuman) const {
    humanPlayer[player] = isHuman;
    return this;
  }

  /** 
   * Set computer delay before and after move.
   * 
   * @param delay 
   * 
   * @return this
   */
  auto setComputerDelay(int delay) const {
    computer_delay = delay;
    return this;
  }

  /** 
   * Sets the number of games
   * 
   * @param numGames 
   * 
   * @return this
   */
  auto setNumGames(int numGames) const {
    num_games = numGames;
    return this;
  }

  /** 
   * Sets clearing screen before printing next position.
   * 
   * @param clear 
   * 
   * @return this
   */
  auto setClearScreenBbeforePrinting(bool clear) const {
    Board::clear_screen_before_printing = clear;
  }

  /** 
   * Reports current settings
   * 
   * 
   * @return this
   */
  const MainLoop* reportSettings() const ;

private:
  MainLoop() = default;

  static int  max_depth[2]; ; /**< Max. depth for minmax play for each player*/
  static bool humanPlayer[2]; /**< Which player is human? */
  static int  num_games;      /**< Number of games to play */
  static int  computer_delay; /**< Number of seconds to wait after computer move */
  static const StaticEvaluatorTable& DEFAULT_EVALUATOR_TABLE;

public:
  static int run(std::istream& ins,
		 std::ostream& os,
		 std::ostream& logs,
		 const StaticEvaluatorTable& evaluatorTab = DEFAULT_EVALUATOR_TABLE);

  static int run(const StaticEvaluatorTable& evaluatorTab = DEFAULT_EVALUATOR_TABLE);

private:

  static int play(int game, const StaticEvaluatorTable& evaluatorTab);
};

#endif /* MAIN_LOOP */
