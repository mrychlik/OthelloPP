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


class Board {
public:
  typedef std::tuple<int, int, Board> move_type;
  typedef std::vector<move_type> move_bag_type;

public:
  Board();
  int value() const;
  int numTiles () const;
  move_bag_type moves(bool playWhite) const;
  int score() const;
  bool isWhitesTurn() const;
  bool operator==(const Board&) const;
  bool anyLegalMoves(bool playWhite) const;

private:

  bool isLegal(bool playWhite , int x, int y) const;

  operator std::string() const;
  
  friend std::ostream& operator<<(std::ostream& s, const Board& b);

public:

  std::ostream& print(std::ostream& s, bool big = false) const;

private:
  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;

  int score_         : 8;	/**< Score */
  int whitesTurn_    : 1;	/**< Is it White's turn? */
  int numTiles_      : 7;	/**< Number of tiles on board */

  /**
   * Packed bit array telling us which squares
   * are occupied (row is y).
   */
  uint64_t filled_;		

  /**
   * Packaged bit array telling us which squares
   * have a white piece on them.
   */
  uint64_t white_;

  bool isFilled(uint8_t x, uint8_t) const; 
  bool isWhite(uint8_t x, uint8_t) const; 
  void setWhite(uint8_t x, uint8_t); 
  void setBlack(uint8_t x, uint8_t); 
};




#endif // BOARD_HPP
