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
#include "StaticEvaluator.hpp"

/**
 * This class runs the game loop and controls 
 * numerous game settings. It provides "fluent" style interface for ease of use,
 * e.g.:
 *
 * MainLoop::getInstance()
 *   .setMaxDepth(Board::WHITE, 12)
 *   .setMaxDepth(Board::BLACK, 12)  
 *   .setNumGames(1)
 *   .run();
*/
class MainLoop {
private:

  static const int DEFAULT_NUM_GAMES = 10; /**< Number of games to play by default */
  static const int DEFAULT_MAX_DEPTH = 12; /**< Depth to which examine the tree to compute the best move */
  static const int DEFAULT_COMPUTER_DELAY = 0; /**< Amount of delay in sec. after computer move */

public:

  /** 
   * Get pointer to the only instance of this class.
   * 
   * 
   * @return 
   */
  static const MainLoop& getInstance();
  
  /** 
   * Set game depth for a player.
   * 
   * @param player 
   * @param depth 
   * 
   * @return *this
   */
  const MainLoop& setMaxDepth(BoardTraits::Player player, int depth) const;

  /** 
   * Designates player to be human, or otherwise
   * capable of interactive play.
   * 
   * @param player 
   * @param isHuman 
   * 
   * @return *this
   */
  const MainLoop& setHumanPlayer(BoardTraits::Player player, bool isHuman) const;

  /** 
   * Set computer delay before and after move.
   * 
   * @param delay 
   * 
   * @return *this
   */
  const MainLoop& setComputerDelay(int delay) const;

  /** 
   * Sets the number of games
   * 
   * @param numGames 
   * 
   * @return *this
   */
  const MainLoop& setNumGames(int numGames) const; 

  /** 
   * Set board print size to big or small
   * 
   * @param printBig 
   * 
   * @return 
   */
  const MainLoop& setPrintSizeBig(bool printBig) const;

  /** 
   * Sets clearing screen before printing next position.
   * 
   * @param clear 
   * 
   * @return *this
   */
  const MainLoop& setClearScreenBbeforePrinting(bool clear) const;

  /** 
   * Sets board width
   * 
   * @param width 
   * 
   * @return 
   */
  const MainLoop& setBoardWidth(int width) const;

  /** 
   * Sets board height
   * 
   * @param width 
   * 
   * @return 
   */
  const MainLoop& setBoardHeight(int width) const;


  /** 
   * Reports current settings
   * 
   * 
   * @return *this
   */
  const MainLoop& reportSettings() const ;

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
