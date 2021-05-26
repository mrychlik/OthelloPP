/**
 * @file   SimpleStaticEvaluator.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 23:21:49 2021
 * 
 * @brief  A trivial static evaluator
 * 
 * 
 */

#ifndef SIMPLE_STATIC_EVALUATOR_HPP
#define SIMPLE_STATIC_EVALUATOR_HPP

#include "StaticEvaluator.hpp"
#include "Board.hpp"

/**
 * Uses score as value of the board. 
 * 
 */
struct SimpleStaticEvaluator : public StaticEvaluator, public StaticEvaluatorTraits
{
  value_type operator()(const Board& b, BoardTraits::Player player, int depth) const
  {
    return b.score();
  }
};

#endif 
