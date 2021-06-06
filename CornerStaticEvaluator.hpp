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
class CornerStaticEvaluator : public StaticEvaluator, private StaticEvaluatorTraits
{
public:
  static const int DEFAULT_CORNER_VALUE = 8; /**< Default value for corner */

  /** 
   * Constructor.
   * 
   * @param cornerVal 
   * 
   * @return 
   */
  CornerStaticEvaluator(int cornerVal = DEFAULT_CORNER_VALUE) : cornerVal(cornerVal) { }

  StaticEvaluatorTraits::value_type operator()(const Board& b, Board::Player player __attribute__((unused)), int depth __attribute__((unused))) const
  {
    auto val = b.score();

    //maybe add linear change to value of score vs terriory?


    if( b.isFilled(0,0) ) {
      val += b.isWhite(0,0) ? cornerVal : -cornerVal;
    }
    if( b.isFilled(0, Board::h()-1) ) {
      val += b.isWhite(0,Board::h()-1) ? cornerVal : -cornerVal;
    }
    if( b.isFilled(Board::w()-1, 0) ) {
      val += b.isWhite(Board::w()-1,0) ? cornerVal :  -cornerVal;
    }
    if( b.isFilled(Board::w()-1, Board::h()-1) ) {
      val +=  b.isWhite(Board::w()-1,Board::h()-1) ? cornerVal : -cornerVal;
    }
    return val;
  }

private:

  int cornerVal; /**<  how much more valuable is a corner than any other flip */

};

#endif
