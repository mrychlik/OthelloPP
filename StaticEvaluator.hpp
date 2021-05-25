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


#include <limits>

struct StaticEvaluatorTraits {
  /** 
   * Board value type
   * 
   */
  typedef int value_type;

  /**
   * The top of the node value range. 
   * 
   */
  static const value_type MAX_VAL = std::numeric_limits<value_type>::max();

  /**
   * The bottom of the node value range.
   * 
   */
  static const value_type MIN_VAL = std::numeric_limits<value_type>::min();
};


/**
 * We use int as return value of functions that in principle should
 * return values in the range [MIN_VAL,MAX_VAL]. If this
 * changed, things would break. The rationale for small
 * Board::value_type is that we may want to cache it in Board
 * instances at some point.
 * 
 */
static_assert(sizeof(StaticEvaluator::value_type) <= sizeof(int));


/**
 * Abstract base class of all static evaluators
 * 
 */
struct StaticEvaluator {

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
