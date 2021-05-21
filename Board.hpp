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
#include <deque>
#include <iosfwd>
#include <cinttypes>
#include <vector>
#include <tuple>


/** 
 * Board class represents the board of the Othello game.
 * This class is responsible for:
 *
 *    1. For every square of the board it knows
 *       if the square is occupied (filled) and
 *       what color is the piece
 *    2. Generation of moves from any position
 *    3. Keeping track whose turn it is (white or black)
 *    4. Keeping track of score (the number of white
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
  typedef std::vector<move_type> move_bag_type;

public:
  Board();
  int value() const;
  int numTiles() const;
  move_bag_type moves(bool playWhite) const;
  int score() const;
  bool isWhitesTurn() const;
  bool operator==(const Board&) const;
  bool hasLegalMove(bool playWhite) const;

private:

  bool isLegal(bool playWhite , int x, int y) const;

  operator std::string() const;
  
  friend std::ostream& operator<<(std::ostream& s, const Board& b);

public:

  /** 
   * Prints the board to an ostream.
   * 
   * @param s 
   * @param big   If true, the board is printed in large size
   * 
   * @return 
   */
  std::ostream& print(std::ostream& s, bool big = false) const;

private:
  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;

  int score_         : 8;	/**< Score */
  int whitesTurn_    : 1;	/**< Is it White's turn? */
  int numTiles_      : 7;	/**< Number of tiles on board */

  uint64_t filled_;		/**< Bits telling us if a square is occupied */
  uint64_t white_;		/**< Bits telling us if a square is occupied by a white piece  */

  bool isFilled(uint8_t x, uint8_t y) const; 
  bool isWhite(uint8_t x, uint8_t y) const;
  bool isBlack(uint8_t x, uint8_t y) const;   

  void setWhite(uint8_t x, uint8_t y); 
  void setBlack(uint8_t x, uint8_t y); 
};




#endif // BOARD_HPP
