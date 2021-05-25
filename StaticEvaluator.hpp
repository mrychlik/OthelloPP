/**
 * @file   StaticEvaluator.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 23:00:24 2021
 * 
 * @brief  Defines abstract base class of all evaluators
 * 
 * 
 */

#ifndef STATIC_EVALUATOR_HPP
#define STATIC_EVALUATOR_HPP

/**
 * Abstract base class of all static evaluators
 * 
 */
struct AbstractStaticEvaluator {
  /** 
   * @param b
   * @param player
   * @param depth
   *
   * @return 
   */
  virtual int operator()(const Board& b, Board::Player player, int depth) const = 0;
};


#endif
