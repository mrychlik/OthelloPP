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
struct StaticEvaluator {
  /** 
   * Board value type
   * 
   */
  typedef int value_type;

  /** 
   * @param b
   * @param player
   * @param depth
   *
   * @return 
   */
  virtual int operator()(const Board& b, Board::Player player, int depth) const = 0;
};

/**
 * We use int as return value of functions that in principle should
 * return values in the range [Board::MIN_VAL,Board::MAX_VAL]. If this
 * changed, things would break. The rationale for small
 * Board::value_type is that we may want to cache it in Board
 * instances at some point.
 * 
 */
static_assert(sizeof(StaticEvaluator::value_type) <= sizeof(int));


#endif
