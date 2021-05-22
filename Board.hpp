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

enum Player {
  BLACK =  0,
  WHITE =  1,
};

inline Player operator~(Player player) {
  return player == WHITE ? BLACK : WHITE;
}


/** 
 * Board class represents the board of the Othello game.
 * This class is responsible for:
 *
 *    1. For every square of the board it knows
 *       if the square is occupied (filled) and
 *       what color is the piece
 *    2. Generation of moves from any position
 *    3. Keeping track whose turn it is (white or black)
 *    4. Computint the score (the number of white
 *       minus the number of black pieces)
 *    5. Computing heuristic value of the board
 *       position.
 * 
 */
class Board {
public:
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
  typedef int8_t value_type;
public:
  Board();
  value_type value() const;
  int numTiles() const;
  int numWhiteTiles () const;
  int numBlackTiles () const;  
  move_bag_type moves(Player player) const;
  int score() const;
  bool hasLegalMove(Player player) const;

  // Use autogenerated comparison; since C++20
  friend bool operator==(const Board&, const Board&) = default;

private:

  operator std::string() const;
  friend std::ostream& operator<<(std::ostream& s, const Board& b);

public:
  
  std::ostream& print(std::ostream& s, bool big = false) const;

private:

  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;

  uint8_t filled[8];		/**< Is a square is occupied? */
  uint8_t white[8];		/**< Is a square occupied by white? */

  bool isFilled(uint8_t x, uint8_t y) const; 
  bool isWhite(uint8_t x, uint8_t y) const;
  bool isBlack(uint8_t x, uint8_t y) const;   

  void setWhite(uint8_t x, uint8_t y); 
  void setBlack(uint8_t x, uint8_t y); 
  void setColor(uint8_t x, uint8_t y, bool colorWhite); 

  bool findFlipRadius(Player player, uint8_t x, uint8_t y, uint8_t flipRadius[8]) const;
  void toggleTurn();
};




#endif // BOARD_HPP
