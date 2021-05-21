/**
 * @file   Board.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 13:51:37 2021
 * 
 * @brief  Board implementation
 * 
 * 
 */

#include "Board.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

static const char esc = '';
static const std::string reset = "[0m";

/**
 * 8 directions  (vectors) on the board.
 * 
 */
static const int direction[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

bool Board::isFilled(int x, int y) const {
  return (filled_ >> (8 * y + x)) & 1U;
};

bool Board::isWhite(int x, int y) const {
  return (white_ >> (8 * y + x) ) & 1UL;
};

void Board::flipToWhite(int x, int y) {
  filled_ |= 1UL << (8 * y + x);
  white_  |= 1UL << (8 * y + x);
};

void Board::flipToBlack(int x, int y) {
  filled_ |= 1UL << (8 * y + x);
  white_  &= ~ ( 1UL << (8 * y + x) );
};

Board::Board() :
  score_(0),
  whitesTurn_(true),		
  numTiles_(0),			//4 pieces on Board  
  filled_(0),			
  white_(0)		
{
  // Standard Othello board initialization
  flipToWhite(3,4);
  flipToWhite(4,3);
  flipToBlack(3,3);
  flipToBlack(4,3);  
}
  


/** 
 * # of white tiles - # black tiles
 * 
 * 
 * @return 
 */
int Board::score () const { 
  return score_;
}

/** 
 * Static evaluator?
 * 
 * 
 * @return 
 */
int Board::value() const {
  int value = score();

  //maybe add linear change to value of score vs terriory?

  const int cornerVal = 8; //how much more valuable is a corner than any other flip
  if( isFilled(0,0) ) {
    value += isWhite(0,0) ? cornerVal : -cornerVal;
  }
  if( isFilled(0,7) ) {
    value += isWhite(0,7) ? cornerVal : -cornerVal;
  }
  if( isFilled(7,0) ) {
    value += isWhite(7,0) ? cornerVal :  -cornerVal;
  }
  if( isFilled(7,7) ) {
    value +=  isWhite(7,7) ? cornerVal : -cornerVal;
  }
  return value;
}

int Board::numTiles () const {
  return numTiles_;
}

/** 
 * Generate all moves.
 * 
 * @param playWhite 
 * 
 * @return A list of triples (x, y, board) such
 *         that board is the board after a valid move (x, y)
 *         is made.
 */
Board::move_bag_type
Board::moves(bool playWhite) const
{
  move_bag_type move_bag;
  for( auto x = 0; x < 8; ++x) {
    for( auto y = 0; y < 8; ++y) {
      if( isFilled(x,y) ) continue;
      int toFlip[8];
      for (int ray = 0; ray < 8; ray++) { //iter over cardinal + diagonals
	auto distance = 1;
	auto end = 0;
	while (end == 0) {
	  auto tmpx = x + distance * direction[ray][0];
	  auto tmpy = y + distance * direction[ray][1];

	  if ( tmpx < 0 || tmpx > 7 || tmpy < 0 || tmpy > 7 ) {
	    end = 1;		//ran off edge
	  } else if( !isFilled(tmpx,tmpy) ) {
	    end = 2;		//ran into an empty space
	  } else if( isWhite(tmpx,tmpy) == playWhite) {
	    end = (distance > 1)? 3 : 4; //ran into own color late vs early
	  } else {
	    distance++;
	  }
	}
	if (end == 3) {
	  toFlip[ray] = distance;
	} else {
	  toFlip[ray] = 0;
	}
      } //checked all rays and populated toFlip

      bool legal = false;
      for (int i : toFlip) { //any legal flips from playing here?
	if (i != 0) {
	  legal = true;
	  break;
	}
      }

      if (legal) {
	int numFlipped = 0;
	Board c(*this);

	for (int r = 0; r < 8; r++) { //rays, must be at least 1 that is > 0
	  if ( toFlip[r] != 0 ) {
	    numFlipped += toFlip[r]-1;
	  }

	  for (int d = 1; d < toFlip[r]; d++) { //above calced length
	    if( playWhite ) {
	      c.flipToWhite(x + d * direction[r][0], y + d * direction[r][1]); //update Board c according to color
	    } else {
	      c.flipToBlack(x + d * direction[r][0], y + d * direction[r][1]);
	    }
	  }
	}
	
	if( playWhite ) {
	  c.flipToWhite(x,y);	//place new tile
	} else {
	  c.flipToBlack(x,y);
	}

	numFlipped = 2*numFlipped + 1; //account for placed tile and that score -1 for lost white and -1 from new black of each flip. thus 2*flip + 1

	c.numTiles_ = numTiles_ + 1;
	if (!playWhite) {
	  // change turn back. if just played white, then its B's turn and no change
	  c.whitesTurn_ = 1; 
	  c.score_ = score_ - numFlipped; //play black, score decreases
	} else {
	  c.score_ = score_ + numFlipped; //play white, score increases
	}
	//c.numTiles = tn;
	move_bag.push_back(move_type(x, y, c));
      }
    }
  }
  return move_bag;
}

std::ostream& operator<<(std::ostream& s, const Board& b) {
  return b.print(s, false);
};



std::ostream& Board::print(std::ostream& s, bool big) const {
  if (big) {
    return printBig(s);
  } else
    return printSmall(s);
}

std::ostream& Board::printSmall(std::ostream& s) const {
  s << " 01234567\n";
  for (int y = 0; y < 8; y++) {
    s << y;
    for (int x = 0; x < 8; x++) {
      auto bg   = (y%2 == x%2) ? "42" : "43";
      auto fg   = isWhite(x,y) ? "37" : "30";
      auto tile = isWhite(x,y) ? "W"  : "B";
      if( !isFilled(x,y) ) tile = " ";
      //out += esc + "[" + fg + ";" + bg + "m" + tile;
      s << esc << '[' << fg << ';' << bg << 'm' << tile;
    }
    s << esc << "[0m" << y << "\n";
  }

  s<< " 01234567\n";
  return s;
}

std::ostream& Board::printBig(std::ostream& s) const {
  s << esc << "[1;1H" << esc
    << "[0J"
    << "  0  1  2  3  4  5  6  7 \n";

  for (int y = 0; y < 8; y++) {
    s << " ";
    for (int c = 0; c < 8; c++) {
      auto bg = (c%2 == y%2)? "42":"43";
      s << esc << "[;" << bg << "m   ";
    }  
    s << reset << "\n" << y;
    for (int x = 0; x < 8; x++) {
      auto bg = (y%2 == x%2)? "42":"43";
      auto fg = (isWhite(x,y))? "37":"30";
      auto tile = " "; // consider revising
      if (isFilled(x,y)){
	tile = (isWhite(x,y))? "W":"B"; 
      } else tile = " ";
      s << esc << '[' << fg << ';' << bg
	<< "m " << tile << " ";
    }
    s << reset << y << "\n"
      << " ";
    for (int c = 0; c < 8; c++) {
      auto bg = (c%2 == y%2)? "42":"43";
      s << esc
	<< '['
	<< ";" << bg //no fg
	<< 'm'
	<< "   ";
    }  
    s << reset << "\n";
  }
  s << "  0  1  2  3  4  5  6  7 \n";
  return s;
}

bool Board::isWhitesTurn() const {
  return whitesTurn_;
}


bool Board::anyLegalMoves(bool playWhite) const {
  return !moves(playWhite).empty();
}

bool Board::operator==(const Board& b) const {
  // NOTE: IntScore is omitted from the comparison
  return  whitesTurn_ == b.whitesTurn_
    && numTiles_ == b.numTiles_
    && filled_ == b.filled_
    && white_ == b.white_;
}

Board::operator std::string() const {
  std::stringstream buf;
  buf << *this;
  return buf.str();
}
