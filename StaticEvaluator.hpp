/**
 * @file   StaticEvaluator.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 21:28:15 2021
 * 
 * @brief  These are static evaluators
 * 
 * 
 */

struct StaticEvaluatorSimple : public StaticEvaluator
{
  int opertor()(const Board& b)  const
  {
    return b->score();
  }
};

struct StaticEvaluatorWithCorners : StaticEvaluator
{
  int opertor()(const Board& b)  const
  {
    auto val = score();

    // NOTE: (M.R.) This handicap seems to disproportionately award WHITE.
    //maybe add linear change to value of score vs terriory?

    static const int cornerVal = ::ceil(0.3 * ( numTiles() - 20)); //how much more valuable is a corner than any other flip
    if( isFilled(0,0) ) {
      val += isWhite(0,0) ? cornerVal : -cornerVal;
    }
    if( isFilled(0,7) ) {
      val += isWhite(0,7) ? cornerVal : -cornerVal;
    }
    if( isFilled(7,0) ) {
      val += isWhite(7,0) ? cornerVal :  -cornerVal;
    }
    if( isFilled(7,7) ) {
      val +=  isWhite(7,7) ? cornerVal : -cornerVal;
    }
    return val;
  }
};
