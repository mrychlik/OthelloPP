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

static inline
uint32_t popcount(const uint8_t x[8])
{
  // We use a non-portable, GCC specific function
  // but there are many portable implementations
  // which are quite efficient
  unsigned long u0, u1;
  u0 = (x[0] << 0) | (x[1] << 8) | (x[2] << 16) | (x[3] << 24);
  u1 = (x[4] << 0) | (x[5] << 8) | (x[6] << 16) | (x[7] << 24);  
  unsigned long u = u0 | (u1 << 32);

  return __builtin_popcountl(u);
}

/**
 * 8 directions  (vectors) on the board.
 * 
 */
static constexpr int direction[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

bool Board::isFilled(uint8_t x, uint8_t y) const {
  return filled[y] & (0x80 >> x);
};

bool Board::isWhite(uint8_t x, uint8_t y) const {
  return white[y] & (0x80 >> x);
};

void Board::setWhite(uint8_t x, uint8_t y) {
  filled[y] |= (0x80 >> x);
  white[y]  |= (0x80 >> x);
};

void Board::setBlack(uint8_t x, uint8_t y) {
  filled[y] |= (0x80 >> x);
  white[y]  &= ~(0x80 >> x);
};

void Board::setColor(uint8_t x, uint8_t y, bool colorWhite) {
  if( colorWhite ) {
    setWhite(x,y);
  } else {
    setBlack(x,y);
  }
}

Board::Board(Player player) :
  filled{0},			
  white{0}
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
  return numWhiteTiles() - numBlackTiles();
}

/** 
 * Static evaluator?
 * 
 * 
 * @return 
 */
Board::value_type Board::value() const {
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
  return popcount(filled);
}

/** 
 * Returns the number of white tiles on the board.
 * 
 * 
 * @return 
 */
int Board::numWhiteTiles () const {
  return popcount(white);
}

/** 
 * Returns the number of black tiles on the board.
 * 
 * 
 * @return 
 */
int Board::numBlackTiles () const {
  return numTiles() - numWhiteTiles();
}


/** 
 * For a given board square and each of the 8 rays from that square
 * compute number of tiles to flip along that ray. As a side
 * efect, determine whether whe move is legal, in the sense that
 * it would result in flipping some tiles to the player's color.
 * The value returned is true if the move is legal.
 *
 * If shortCircuit is true, however, the calculation
 * is abandoned upon discovering that squares will be flipped
 * and true is returned immediately. In this case, the
 * values in flipRadius array may not valid except for one.
 * 
 * @param player
 * @param x 
 * @param y 
 * @param flipRadius
 * @param shortCircuit
 * 
 * @return True if the move is legal
 */
bool Board::findFlipRadius(Player player, uint8_t x, uint8_t y,
			   uint8_t flipRadius[8],
			   bool shortCircuit) const
{
  bool legal = false;
  for (int ray = 0; ray < 8; ++ray) { //iter over cardinal + diagonals
    uint8_t distance = 1;
    int8_t end = 0;
    while (end == 0) {
      int8_t tmpx = x + distance * direction[ray][0];
      int8_t tmpy = y + distance * direction[ray][1];

      if ( tmpx < 0 || tmpx > 7 || tmpy < 0 || tmpy > 7 ) {
	end = 1;		//ran off edge
      } else if( !isFilled(tmpx,tmpy) ) {
	end = 2;		//ran into an empty space
      } else if( isWhite(tmpx, tmpy) == (player == WHITE) ) {
	end = ( distance > 1 ) ? 3 : 4; //ran into own color late vs early
      } else {
	++distance;
      }
    }
    if (end == 3) {
      flipRadius[ray] = distance;
      legal = true;
      if(shortCircuit) {
	// We determined that move is legal
	// and abandon further calculatons
	break;
      }
    } else {
      flipRadius[ray] = 0;
    }
  } 
  return legal;
}

/** 
 * Generate all moves.
 * 
 * @param playWhite 
 * 
 * @return A list of triples (x, y, board) such
 *         that board is the board after a valid move (x, y)
 *         is made.
 *
 * @throw std::bad_alloc
 */
Board::move_bag_type
Board::moves(Player player) const
{
  move_bag_type move_bag;
  for( auto x = 0; x < 8; ++x) {
    for( auto y = 0; y < 8; ++y) {
      if( isFilled(x,y) ) continue;
      uint8_t flipRadius[8];
      auto legal = findFlipRadius(player, x, y, flipRadius);
      if (legal) {
	Board c(*this);
	for (int r = 0; r < 8; r++) { 
	  for (int d = 1; d < flipRadius[r]; d++) { 
	    c.setColor(x + d * direction[r][0], y + d * direction[r][1], player); 
	  }
	}
	c.setColor(x,y, player);	//place new tile
	move_bag.emplace_front(x, y, c); // Here is where std::bad_alloc would be thrown
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
  s << "Static value: " << value()    
    << "\nNumber of tiles: " << numTiles()
    << "\nNumber of white tiles: " << numWhiteTiles()    
    << "\nNumber of black tiles: " << numBlackTiles()    
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
      auto bg = (y%2 == x%2) ? "42" : "43";
      auto fg = isWhite(x,y) ? "37" : "30";
      auto tile = isFilled(x,y)
	? ( isWhite(x,y) ? "W" : "B")
	: " "; 
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
 * Predicate: Is there at least one legal move?
 *
 * @param playWhite
 * 
 * @return 
 */
bool Board::hasLegalMove(Player player) const {
  uint8_t flipRadius[8];
  for(auto x = 0; x < 8; ++x)
    for(auto y = 0; y < 8; ++y)    
      if( findFlipRadius(player, x, y, flipRadius, true) )
	return true;
  return false;
}

/** 
 * Convert Board to a string.
 * 
 * 
 * @return A string with terminal control characters
 *         resulting in a printed board with pieces.
 */
Board::operator std::string() const {
  std::stringstream buf;
  buf << *this;
  return buf.str();
}
