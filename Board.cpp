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
#include <iomanip>
#include <cassert>
#include <cmath>

static const char esc = '';
static const std::string reset = "[0m";


/**
 * 8 directions  (vectors) on the board.
 * 
 */
static constexpr int direction[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

/**
 * Controls the size of the printed board.
 * 
 */
bool Board::print_size_big = true;

bool Board::clear_screen_before_printing = false;


/** 
 * Checks if coordinates are in range.  It works with a signed type as
 * in calculations we may produce invalid coordinates by
 * icrement/decrement, etc.
 * 
 * @param x 
 * @param y 
 * 
 * @return 
 */
bool Board::validCoords(int8_t x, int8_t y) const {
  return ( x >= 0 ) && ( x < w() ) && ( y >= 0 ) && ( y < h() );
}


/** 
 * Set tile at (x,y) to a particular color
 * 
 * @param x 
 * @param y 
 * @param player
 */
void Board::setColor(uint8_t x, uint8_t y, Player player) {
  if(player == WHITE ) {
    setWhite(x,y);
  } else {
    setBlack(x,y);
  }
}

/** 
 * Constructs default board 
 */
Board::Board() :
  filled{0},			
  white{0}
{
  // Board sizes of 4, 6, 8 are supported, others are not
  assert( (w() >= 4) && ( h() >= 4 ) && ( w() <= 8) && ( h() <= 8) );
  assert( w() % 1 == 0);	// Board width must be even; why?
  assert( h() % 1 == 0);	// Board height must be even; why?

  auto half_width = w() / 2;
  auto half_height = h() / 2;
  // Standard Othello board initialization
  setWhite(half_width, half_height - 1);
  setWhite(half_width-1, half_height);
  setBlack(half_width, half_height);
  setBlack(half_width-1, half_height - 1);  
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
  assert(!isFilled(x, y));

  bool legal = false;
  for (int ray = 0; ray < 8; ++ray) { //iter over cardinal + diagonals
    int8_t end = 0;
    uint8_t distance = 1;
    for(/* Empty */; end == 0; ++distance) {
      int8_t tmpx = x + distance * direction[ray][0];
      int8_t tmpy = y + distance * direction[ray][1];

      if ( !validCoords(tmpx, tmpy) ) {
	end = 1;		//ran off edge
      } else if( !isFilled(tmpx,tmpy) ) {
	end = 2;		//ran into an empty space
      } else if( isWhite(tmpx, tmpy) == (player == WHITE) ) {
	end = ( distance > 1 ) ? 3 : 4; //ran into own color late vs early
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
 * @param player
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
  for( auto x = 0; x < w(); ++x) {
    for( auto y = 0; y < h(); ++y) {
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
  if( Board::clear_screen_before_printing) {
    s << "[2J";
  }
  b.print(s, Board::print_size_big);
  s << std::flush;
  return s;
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
  auto rowlabel = std::string("01234567").substr(0,w());
  s << " " << rowlabel.substr(0,w()) << "\n";
  for (int y = 0; y < h(); y++) {
    s << y;
    for (int x = 0; x < w(); x++) {
      auto bg   = (y%2 == x%2) ? "42" : "43";
      auto fg   = isWhite(x,y) ? "37" : "30";
      auto tile = isWhite(x,y) ? "W"  : "B";
      if( !isFilled(x,y) ) tile = " ";
      //out += esc + "[" + fg + ";" + bg + "m" + tile;
      s << esc << '[' << fg << ';' << bg << 'm' << tile;
    }
    s << esc << "[0m" << y << "\n";
  }
  s << " " << rowlabel << "\n";
  s << "Score: " << score()
    << "\nNumber of tiles: " << numTiles()
    << "\nNumber of white tiles: " << numWhiteTiles()    
    << "\nNumber of black tiles: " << numBlackTiles()    
    << std::endl;
  return s;
}

std::ostream& Board::printBig(std::ostream& s) const {
  auto rowlabel = std::string("0  1  2  3  4  5  6  7").substr(0, 3*w());
  s << esc << "[1;1H" << esc
    << "[0J"
    << "  " << rowlabel << " \n";

  for (int y = 0; y < h(); y++) {
    s << " ";
    for (int c = 0; c < w(); c++) {
      auto bg = (c%2 == y%2)? "42":"43";
      s << esc << "[;" << bg << "m   ";
    }  
    s << reset << "\n" << y;
    for (int x = 0; x < w(); x++) {
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
    for (int c = 0; c < w(); c++) {
      auto bg = (c%2 == y%2)? "42":"43";
      s << esc
	<< '['
	<< ";" << bg //no fg
	<< 'm'
	<< "   ";
    }  
    s << reset << "\n";
  }
  s << "  " << rowlabel << " \n";
  return s;
}

/** 
 * Predicate: Is there at least one legal move?
 *
 * @param player The player
 * 
 * @return 
 */
bool Board::hasLegalMove(Player player) const {
  uint8_t flipRadius[8];
#if 0
  for(auto x = 0; x < w(); ++x)
    for(auto y = 0; y < h(); ++y)    
      if( !isFilled(x, y) && findFlipRadius(player, x, y, flipRadius, true) )
	return true;
#endif

  uint8_t j = 0;
  for(uint64_t u = ~filled; u != 0; ) {
    auto i = __builtin_clzl(u);
    j += i + 1;
    uint8_t x = j & 0x07;
    uint8_t y = (j >> 3) & 0x07;
    if( findFlipRadius(player, x, y, flipRadius, true) ) {
      return true;
    }
    u <<= i + 1;
  }
  return false;
}

#if 0
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
#endif

uint8_t Board::w_ = 8;
uint8_t Board::h_ = 8;

/** 
 * Set board width
 * 
 * @param w 
 */
void Board::setW(uint8_t w)
{
  if(w > 8 || w % 2 != 0) {
    throw std::logic_error("Unsupported board width");
  }
  Board::w_ = w;
}

/** 
 * Set board height
 * 
 * @param h 
 */
void Board::setH(uint8_t h)
{
  if(h > 8 || h % 2 != 0) {
    throw std::logic_error("Unsupported board height");
  }
  Board::h_ = h;
}

/**
 * 
 * If this is changed sometime in the future, this
 * would break popcount.
 */
static_assert(sizeof(uint64_t) == sizeof(unsigned long));

/** 
 *  We use a non-portable, GCC specific function
 * but there are many portable implementations
 * which are quite efficient. There are issues
 * close to hardware bugs and poor compiler
 * design around them; here is a discussion.
 * https://stackoverflow.com/questions/25078285/replacing-a-32-bit-loop-counter-with-64-bit-introduces-crazy-performance-deviati
 *
 * 
 * @param x A 64-it integer
 * 
 * @return 
 */

uint32_t Board::popcount(const uint64_t x)
{
  return __builtin_popcountl(x);
}

#include <cassert>

#if USE_LUT

/**
 * A lookup table of powers of 2 and their bitwise complements.
 *
 * NOTE: Some G++ versions seem to have trouble if there is no brace
 * initializer in the constructor. In practice, the use of lookup
 * table is questionable, as it produces no performance
 * enhancements. Inlining the calculation and presumably doing the
 * arithmetic in registers in setbit will beat the memory access, even
 * if the table is cached near the CPU. Our interest is purely
 * "academic" in this solution.
 *
 */
static constexpr
struct PowersOfTwo {
  constexpr PowersOfTwo() : tbl{0}, ctbl{0} {
    for(int j=0; j < 64; ++j){
      tbl[j] = 1UL << j;
      ctbl[j] = ~tbl[j];
    }
  }
  uint64_t tbl[64];
  uint64_t ctbl[64];
} pot;
#endif

/** 
 * Get bit with index 8*y+x from a 64-bit unsigned integer.
 * Least significant bit has index 0.
 * 
 * @param u 
 * @param x 
 * @param y 
 * 
 * @return The bit as a boolean value.
 */

bool Board::getbit(const uint64_t& u, uint8_t x, uint8_t y)
{
  assert(x < 8);  assert(y < 8);  
#if USE_LUT
  return u & pot.tbl[ (y << 3) | x ]; 
#else
  return ( u >> ( (y << 3) | x ) ) & 1U;
#endif
};


/** 
 * Set bit with index 8*y+x in a 64-bit unsigned integer u.
 * Most significant bit has index 0.
 * 
 * @param u 
 * @param x 
 * @param y 
 * 
 * @return 
 */

void Board::setbit(uint64_t& u, uint8_t x, uint8_t y)
{
  assert(x < 8);  assert(y < 8);  
#if USE_LUT
  u |= pot.tbl[ (y << 3) | x ];
#else
  u |= 1UL << ( (y << 3) | x );
#endif
}

/** 
 * Unset bit with index 8*y+x in a 64-bit unsigned integer u.
 * Most significant bit has index 0.
 * 
 * @param u 
 * @param x 
 * @param y 
 * 
 * @return 
 */

void Board::unsetbit(uint64_t& u, uint8_t x, uint8_t y)
{
  assert(x < 8);  assert(y < 8);  
#if USE_LUT
  u &= pot.ctbl[ (y << 3) | x  ];
#else
  u &= ~( 1UL << ( (y << 3) | x ) );
#endif
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
 * Is the tile at (x,y) white?
 * 
 * @param x 
 * @param y 
 * 
 * @return 
 */
 bool Board::isWhite(uint8_t x, uint8_t y) const {
  return getbit(white, x, y);
};

/** 
 * Is the tile at (x,y) black?
 * 
 * @param x 
 * @param y 
 * 
 * @return 
 */
 bool Board::isBlack(uint8_t x, uint8_t y) const {
  return getbit(filled^white, x, y);
};

/** 
 * Set the tile at (x,y) to white.
 * 
 * @param x 
 * @param y 
 */
 void Board::setWhite(uint8_t x, uint8_t y) {
  setbit(filled, x, y);
  setbit(white, x, y);
};

/** 
 * Set the tile at (x,y) to black
 * 
 * @param x 
 * @param y 
 */
 void Board::setBlack(uint8_t x, uint8_t y) {
  setbit(filled, x, y);
  unsetbit(white, x, y);
};

/** 
 * Is there a tile at (x,y)?
 * 
 * @param x 
 * @param y 
 * 
 * @return 
 */
 bool Board::isFilled(uint8_t x, uint8_t y) const {
  return getbit(filled, x, y);
};
