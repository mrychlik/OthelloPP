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

/**
 * Virtual base class of all static evaluators
 * 
 */
struct StaticEvaluator {
  virtual int operator()(const Board& b, Board::Player player, int depth) const = 0;
};


struct StaticEvaluatorSimple : public StaticEvaluator
{
  int operator()(const Board& b, Board::Player player, int depth) const
  {
    return b.score();
  }
};

struct StaticEvaluatorWithCorners : StaticEvaluator
{
  int operator()(const Board& b)  const
  {
    auto val = b.score();

    // NOTE: (M.R.) This handicap seems to disproportionately award WHITE.
    //maybe add linear change to value of score vs terriory?

    static const int cornerVal = 8; //how much more valuable is a corner than any other flip
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
};

#endif
