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

/** 
 * A table of powers of 2.
 */
struct ShiftTable
{
  constexpr ShiftTable() : values()
  {
    for (auto x = 0; x < 8; ++x)
      for (auto y = 0; y < 8; ++y)
      {
	values[x][y] = 1UL << (8 * y + x);
      }
  }
  uint64_t values[8][8];

  /** 
   * 
   * 
   * 
   * @return 
   */
  constexpr uint64_t operator()(uint8_t x, uint8_t y) const {
    return values[x][y];
  }

};

static constexpr auto shiftTab = ShiftTable();


bool Board::isFilled(uint8_t x, uint8_t y) const {
  return filled_ & shiftTab(x, y);
};

bool Board::isWhite(uint8_t x, uint8_t y) const {
  return white_ & shiftTab(x, y);
};

void Board::setWhite(uint8_t x, uint8_t y) {
  filled_ |= shiftTab(x, y);
  white_  |= shiftTab(x, y);
};

void Board::setBlack(uint8_t x, uint8_t y) {
  filled_ |= shiftTab(x, y);
  white_  &= ~shiftTab(x, y);
};

Board::Board() :
  score_(0),
  whitesTurn_(true),		
  numTiles_(4),			//4 pieces on Board  
  filled_(0),			
  white_(0)		
{
  // Standard Othello board initialization
  setWhite(3,4);
  setWhite(4,3);
  setBlack(3,3);
  setBlack(4,4);  
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

/** 
 * Returns the number of tiles on the board.
 * 
 * 
 * @return 
 */
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
Board::moves() const
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
	  } else if( isWhite(tmpx,tmpy) == isWhitesTurn()) {
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
	    if( isWhitesTurn() ) {
	      c.setWhite(x + d * direction[r][0], y + d * direction[r][1]); //update Board c according to color
	    } else {
	      c.setBlack(x + d * direction[r][0], y + d * direction[r][1]);
	    }
	  }
	}
	
	if( isWhitesTurn() ) {
	  c.setWhite(x,y);	//place new tile
	} else {
	  c.setBlack(x,y);
	}

	numFlipped = 2*numFlipped + 1; //account for placed tile and that score -1 for lost white and -1 from new black of each flip. thus 2*flip + 1

	c.numTiles_ = numTiles_ + 1;
	// change turn back. if just played white, then its B's turn and no change
	c.setWhitesTurn(!isWhitesTurn()); 
	c.score_ = isWhitesTurn() ? score_ + numFlipped : score_ + numFlipped; //play black, score decreases
	move_bag.push_back(move_type(x, y, c));
      }
    }
  }
  return move_bag;
}

/** 
 * 
 * 
 * @param s 
 * @param b 
 * 
 * @return 
 */
std::ostream& operator<<(std::ostream& s, const Board& b) {
  return b.print(s, false);
};



/** 
 * Prints the board to an ostream.
 * 
 * @param s 
 * @param big   If true, the board is printed in large size
 * 
 * @return 
 */
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

  s << " 01234567\n";
  s << "Score: " << score()
    << "\nWhite's turn: " << std::boolalpha << isWhitesTurn()
    << "\nNumber of tiles: " << numTiles()
    << std::endl;
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

/** 
 * Predicate: Is it the white's turn?
 * 
 * 
 * @return 
 */
bool Board::isWhitesTurn() const {
  return whitesTurn_;
}

/** 
 * 
 * 
 * @param playWhite 
 */
void Board::setWhitesTurn(bool playWhite)
{
  whitesTurn_ = playWhite;
}


/** 
 * Predicate: Is there at least one legal move?
 * 
 * @return 
 */
bool Board::hasLegalMove() const {
  return !moves().empty();
}

/** 
 * Compare excluding score.
 * 
 * @param b 
 * 
 * @return 
 */
bool Board::operator==(const Board& b) const {
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
