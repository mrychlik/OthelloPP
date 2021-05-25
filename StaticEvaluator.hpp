/**
 * @file   StaticEvaluator.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 21:28:15 2021
 * 
 * @brief  These are static evaluators
 * 
 * 
 */
#ifndef STATIC_EVALUATOR_HPP
#define STATIC_EVALUATOR_HPP

#include "AbstractStaticEvaluator.hpp"

/**
 * Virtual base class of all static evaluators
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


/**
 * Uses score as value of the board. 
 * 
 */
struct StaticEvaluatorSimple : public StaticEvaluator
{
  int operator()(const Board& b, Board::Player player, int depth) const
  {
    return b.score();
  }
};

/**
 * Uses score and the value of the corners.
 * 
 */
class StaticEvaluatorWithCorners : StaticEvaluator
{
public:
  static const int DEFAULT_CORNER_VALUE = 8; /**< Default value for corner */

  StaticEvaluatorWithCorners(int cornerVal = DEFAULT_CORNER_VALUE) : cornerVal(cornerVal) { }
  int operator()(const Board& b)  const
  {
    auto val = b.score();

    // NOTE: (M.R.) This handicap seems to disproportionately award WHITE.
    //maybe add linear change to value of score vs terriory?


    if( b.isFilled(0,0) ) {
      val += b.isWhite(0,0) ? cornerVal : -cornerVal;
    }
    if( b.isFilled(0,7) ) {
      val += b.isWhite(0,7) ? cornerVal : -cornerVal;
    }
    if( b.isFilled(7,0) ) {
      val += b.isWhite(7,0) ? cornerVal :  -cornerVal;
    }
    if( b.isFilled(7,7) ) {
      val +=  b.isWhite(7,7) ? cornerVal : -cornerVal;
    }
    return val;
  }

private:

  int cornerVal; /**<  how much more valuable is a corner than any other flip */

};

#endif
