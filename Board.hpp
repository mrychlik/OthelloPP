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

class Board {
public:
  Board();
  Board(Board*);
  int score();
  int value();
  Board* move(bool,int,int);
  int tileNum();
  void copy(Board*);

  std::deque<Board*> children(bool);
  bool whitesTurn();
  bool isLegal(bool, int, int);
  bool anyLegalMoves(bool);
  bool operator==(const Board&);

  operator std::string() { return this->toString(); };

private:

  std::string toStringSmall();
  std::string toStringBig();
  std::string toString();
  std::string toString(bool);


  char filled[8];
  char coloredWhite[8];
  char turnAndTile; //first bit is isWhite'sTurn, last 7 are number of tiles on board
  signed char scoreInt;

  bool isFilled(int, int); 
  bool isWhite(int, int); 
  void flipToWhite(int, int); 
  void flipToBlack(int, int); 
};

#endif // BOARD_HPP
