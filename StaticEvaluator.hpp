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


#include "BoardTraits.hpp"

#include <limits>
#include <cinttypes>

/**
 * Provides some traits for all static evaluators
 * 
 */
struct StaticEvaluatorTraits {
  /** 
   * Board value type
   * 
   */
  typedef int8_t value_type;

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
 * changed, things would break. 
 * 
 */
static_assert(sizeof(StaticEvaluatorTraits::value_type) <= sizeof(int));


/**
 * A forward declaration suffices
 * 
 */
class Board;

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
  virtual StaticEvaluatorTraits::value_type operator()(const Board& b, BoardTraits::Player player, int depth) const = 0;
};

/**
 * Provides a separate evaluator for each player.
 * 
 */
typedef const StaticEvaluator* StaticEvaluatorTable[2];

#endif
