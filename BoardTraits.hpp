/**
 * @file   BoardTraits.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Wed May 26 15:07:27 2021
 * 
 * @brief  Properties of the board that are unlikely to change
 * 
 * 
 */

#ifndef BOARD_TRAITS_HPP
#define BOARD_TRAITS_HPP

/**
 * Properties of the board that are unlikely to change
 * 
 */
struct BoardTraits {
  /**
   * A player, definitely not bool.
   * 
   */
  enum Player {
    BLACK =  0,
    WHITE =  1,
  };
};

/** 
 * Toggles player to the opponent.
 * 
 * @param player 
 * 
 * @return The opponent.
 */
inline BoardTraits::Player operator~(BoardTraits::Player player) {
  return player == BoardTraits::WHITE ? BoardTraits::BLACK : BoardTraits::WHITE;
}



#endif
