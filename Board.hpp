/**
 * @file   Board.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 19 13:40:37 2021
 * 
 * @brief  Othello game board
 * 
 * Uses 64-bit unsigned integers to store board state
 */

#ifndef BOARD_HPP
#define BOARD_HPP 1

#include "BoardTraits.hpp"
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
 *    4. Computing the score (the number of white
 *       minus the number of black pieces)
 * 
 */
class Board : public BoardTraits {
public:
  static bool print_size_big;	/**< Use big size for printing if true*/
  static bool clear_screen_before_printing; /**< Clear screen before printing board if true */

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

public:
  Board();
  /** Autogenerated copy constuctor */
  Board(const Board&) = default;



  /** Autogenerated copy assignment */
  Board& operator=(const Board&) = default;

public:
  int score() const;
  bool isFilled(uint8_t x, uint8_t y) const; 
  bool isWhite(uint8_t x, uint8_t y) const;
  bool isBlack(uint8_t x, uint8_t y) const;   
  int numTiles() const;
  move_bag_type moves(Player player) const;
  bool hasLegalMove(Player player) const;

public:

  static void setW(uint8_t w);
  static void setH(uint8_t h);  

  /** 
   * Board width
   * 
   * 
   * @return 
   */
  static uint8_t w()
  {
    return Board::w_;
  }

  /** 
   * Board height
   * 
   * 
   * @return 
   */
  static uint8_t h()
  {
    return Board::h_;
  }

public:

  friend std::ostream& operator<<(std::ostream& s, const Board& b);  
  std::ostream& print(std::ostream& s, bool big = false) const;

private:

  operator std::string() const;

  int numWhiteTiles () const;
  int numBlackTiles () const;  


  
  std::ostream& printSmall(std::ostream& s) const;
  std::ostream& printBig(std::ostream& s) const;


  void setWhite(uint8_t x, uint8_t y); 
  void setBlack(uint8_t x, uint8_t y); 
  void setColor(uint8_t x, uint8_t y, Player player);

  bool validCoords(int8_t x, int8_t y) const;
  bool findFlipRadius(Player player, uint8_t x, uint8_t y, uint8_t flipRadius[8], bool shortCircuit = false) const;

private:

  uint64_t filled;		/**< Is a square is occupied? */
  uint64_t white;		/**< Is a square occupied by white? */

  static uint8_t w_;		/**< Board width */
  static uint8_t h_;		/**< Board height */
};

#endif // BOARD_HPP
