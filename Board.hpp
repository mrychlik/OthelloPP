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

private:


  bool whitesTurn() const;
  bool isLegal(bool playWhite , int x, int y) const;
  bool anyLegalMoves(bool playWhite) const;
  bool operator==(const Board&) const;

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
   * are occupied (by row).
   */
  int8_t filled[8];		

  /**
   * Packaged bit array telling us which squares
   * have a white piece on them.
   */
  int8_t coloredWhite[8];



  bool isFilled(int, int) const; 
  bool isWhite(int, int) const; 
  void flipToWhite(int, int); 
  void flipToBlack(int, int); 
};




#endif // BOARD_HPP
