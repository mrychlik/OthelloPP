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

class Board {
public:
  Board();
  //Board(Board*); Unconventional copy constructor; the default works on POD like this

  int score() const;
  int value() const;
  Board* move(bool,int,int);
  int tileNum() const;

  // void copy(Board*); Only used to implement POD copy, which is unnecessary

  std::deque<Board*> children(bool);

  bool whitesTurn() const;
  bool isLegal(bool, int, int) const;
  bool anyLegalMoves(bool) const;
  bool operator==(const Board&) const;

  operator std::string() const;
  
  friend std::ostream& operator<<(std::ostream& s, const Board& b);

public:

  std::ostream& print(std::ostream& s, bool big = false) const;

private:
  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;

  char filled[8];
  char coloredWhite[8];
  char turnAndTile; //first bit is isWhite'sTurn, last 7 are number of tiles on board
  signed char scoreInt;

  bool isFilled(int, int) const; 
  bool isWhite(int, int) const; 
  void flipToWhite(int, int); 
  void flipToBlack(int, int); 
};




#endif // BOARD_HPP
