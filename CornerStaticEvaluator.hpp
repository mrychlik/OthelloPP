/**
 * @file   CornerStaticEvaluator.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 23:24:58 2021
 * 
 * @brief  Evaluator that values corners
 * 
 * 
 */

#ifndef CORNER_STATIC_EVALUATOR_HPP
#define CORNER_STATIC_EVALUATOR_HPP

#include "StaticEvaluator.hpp"

/**
 * Uses score and the value of the corners.
 * 
 */
class CornerStaticEvaluator : StaticEvaluator
{
public:
  static const int DEFAULT_CORNER_VALUE = 8; /**< Default value for corner */

  CornerStaticEvaluator(int cornerVal = DEFAULT_CORNER_VALUE) : cornerVal(cornerVal) { }

  int operator()(const Board& b, Board::Player player, int depth) const
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
