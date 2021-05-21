/**
 * @file   unit_tests.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Fri May 21 10:11:39 2021
 * 
 * @brief  Unit tests according to the Boost unit testing framework
 * 
 * 
 */

#include "Board.hpp"
#include "GameTree.hpp"

#include <iostream>
#include <cstring>
#include <cmath>

#include <boost/test/unit_test.hpp>
//#include <boost/test/auto_unit_test.hpp>

/** 
 * 
 * 
 * @param test_addition 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_addition)
{
  BOOST_CHECK( 2 + 2 == 4 );
}

/** 
 * 
 * 
 * @param test_board_print 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_print)
{
  Board b;
  std::cout << b;
  
}

/** 
 * 
 * 
 * @param test_board_print_big 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_print_big)
{
  Board b;
  b.print(std::cout, true);
}

/** 
 * 
 * 
 * @param test_board_print_small 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_print_small)
{
  Board b;
  b.print(std::cout, false);
}

/** 
 * 
 * 
 * @param test_board_move 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_move)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves();
  for( auto& m : move_bag) {
    std::cout << std::get<0>(m) << "\n"
	      << std::get<1>(m) << "\n"
	      << std::get<2>(m) << "\n";
  }
}

/** 
 * 
 * 
 * @param test_board_move_fancy 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_move_fancy)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves();
  for( auto& m : move_bag) {
    auto [x, y, board] = m;
    std::cout << "x: " << x << ", "
	      << "y: " << y << "\n"
	      << board << "\n";
  }
}

/** 
 * 
 * 
 * @param test_board_move_text 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_board_move_text)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves();
  for( auto& m : move_bag) {
    auto [x, y, board] = m;
    std::cout << "x: " << x << ", "
	      << "y: " << y << "\n";
  }
}

/** 
 * 
 * 
 * @param test_expand_one_level 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_expand_one_level)
{
  TreeNode root;
  root.expandOneLevel();
  std::cout << root;
}

/** 
 * 
 * 
 * @param test_tree_evaluate 
 * 
 * @return 
 */
BOOST_AUTO_TEST_CASE(test_tree_evaluate)
{
  for(auto depth = 0; depth < 4; ++depth) {
    TreeNode root;

    auto val = root.evaluate(depth);
    std::cout << "\nDepth : " << depth << "\n"
	      << root
	      << "\nValue: " << static_cast<int>(val) 
	      << std::endl;
  }
}
