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

static const int order[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

bool Board::isFilled(int x, int y) const {
  return (filled[y] & (0x80 >> x));
};

bool Board::isWhite(int x, int y) const {
  return (coloredWhite[y] & (0x80 >> x));
};

void Board::flipToWhite(int x, int y) {
  filled[y] = filled[y] | (0x80 >> x);
  coloredWhite[y] = coloredWhite[y] | (0x80 >> x);
};

void Board::flipToBlack(int x, int y) {
  filled[y] |= 0x80 >> x;
  coloredWhite[y] &= ~(0x80 >> x);
};

Board::Board() :
  scoreInt(0),
  turn(1),			// white's turn and 
  numTiles(4),			//4 pieces on Board  
  filled(),			// Necessary for 0 initialization
  coloredWhite()		// Ditto
{
  // Standard Othello board initialization
  filled[3] = filled[4] = 0b00011000;
  coloredWhite[3] = 0b00001000;
  coloredWhite[4] = 0b00010000;
}
  


/** 
 * # of white tiles - # black tiles
 * 
 * 
 * @return 
 */
int Board::score () const { 
  return this->scoreInt;
}

/** 
 * Static evaluator?
 * 
 * 
 * @return 
 */
int Board::value() const {
  int value = this->scoreInt;

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

int Board::tileNum () const {
  return numTiles;
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
      int tmpx, tmpy, distance, end;
      int toFlip[8];
      for (int ray = 0; ray < 8; ray++) { //iter over cardinal + diagonals
	distance = 1;
	end = 0;
	while (end == 0) {
	  tmpx = x + distance * order[ray][0];
	  tmpy = y + distance * order[ray][1];

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
	      c.flipToWhite(x + (d*order[r][0]), y + (d*order[r][1])); //update Board c according to color
	    } else {
	      c.flipToBlack(x + (d*order[r][0]), y + (d*order[r][1]));
	    }
	  }
	}
	
	if( playWhite ) {
	  c.flipToWhite(x,y);	//place new tile
	} else {
	  c.flipToBlack(x,y);
	}

	numFlipped = 2 * numFlipped + 1; //account for placed tile and that score -1 for lost white and -1 from new black of each flip. thus 2*flip + 1

	c.numTiles = numTiles + 1;
	if (!playWhite) {
	  // change turn back. if just played white, then its B's turn and no change
	  c.turn = 1; 
	  c.scoreInt = scoreInt - numFlipped; //play black, score decreases
	} else {
	  c.scoreInt = scoreInt + numFlipped; //play white, score increases
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

bool Board::whitesTurn() const {
  return turn;
}


bool Board::anyLegalMoves(bool playWhite) const {
  return !moves(playWhite).empty();
}

bool Board::operator==(const Board& b) const {
  // NOTE: IntScore is omitted from the comparison
  return  turn == b.turn
    && numTiles == b.numTiles
    && std::equal(filled, b.filled, filled + 8)
    && std::equal(coloredWhite, b.coloredWhite, coloredWhite + 8);
}

Board::operator std::string() const {
  std::stringstream buf;
  buf << *this;
  return buf.str();
}
