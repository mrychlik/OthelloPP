/**
 * @file   Board.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 13:40:37 2021
 * 
 * @brief  Othello game board
 * 
 * 
 */

#ifndef BOARD_HPP
#define BOARD_HPP 1

#include <string>
#include <iosfwd>
#include <cinttypes>
#include <forward_list>
#include <tuple>


/** 
 * Board class represents the board of the Othello game.
 * This class is responsible for:
 *
 *    1. For every square of the board it knows
 *       if the square is occupied (filled) and
 *       what color is the piece
 *    2. Generation of moves from any board position for each player
 *    4. Computint the score (the number of white
 *       minus the number of black pieces)
 *    5. Computing heuristic (static) value of the board
 *       position.
 * 
 */
class Board {
public:
  enum Player {
    BLACK =  0,
    WHITE =  1,
  };

  /** 
   * Type move_type is a triple (x, y, Board)
   * where x, y are the coordinates of the new piece
   * and Board is a resulting board from the move
   */
  typedef std::tuple<int, int, Board> move_type;

  /** 
   * A collection of moves
   * 
   */
  typedef std::forward_list<move_type> move_bag_type;

  /** 
   * Board value type
   * 
   */
  typedef int value_type;
public:
  Board(Player player = WHITE);
  int value() const;
  int numTiles() const;
  move_bag_type moves(Player player) const;
  int score() const;
  bool hasLegalMove(Player player) const;
  move_type getHumanMove(std::istream& s, const Board::move_bag_type& move_bag)


  // Use autogenerated comparison; since C++20
  friend bool operator==(const Board&, const Board&) = default;

private:

  operator std::string() const;
  friend std::ostream& operator<<(std::ostream& s, const Board& b);
  int numWhiteTiles () const;
  int numBlackTiles () const;  


public:
  
  std::ostream& print(std::ostream& s, bool big = false) const;

private:
  
  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;

  bool isFilled(uint8_t x, uint8_t y) const; 
  bool isWhite(uint8_t x, uint8_t y) const;
  bool isBlack(uint8_t x, uint8_t y) const;   

  void setWhite(uint8_t x, uint8_t y); 
  void setBlack(uint8_t x, uint8_t y); 
  void setColor(uint8_t x, uint8_t y, bool colorWhite); 

  bool findFlipRadius(Player player, uint8_t x, uint8_t y, uint8_t flipRadius[8], bool shortCircuit = false) const;

private:

  uint64_t filled;		/**< Is a square is occupied? */
  uint64_t white;		/**< Is a square occupied by white? */

};

/** 
 * Toggles player to the opponent.
 * 
 * @param player 
 * 
 * @return The opponent.
 */
inline Board::Player operator~(Board::Player player) {
  return player == Board::WHITE ? Board::BLACK : Board::WHITE;
}

/**
 * We use int as return value of functions that in principle should
 * return values in the range [Board::MIN_VAL,Board::MAX_VAL]. If this
 * changed, things would break. The rationale for small
 * Board::value_type is that we may want to cache it in Board
 * instances at some point.
 * 
 */
static_assert(sizeof(Board::value_type) <= sizeof(int));


#endif // BOARD_HPP
