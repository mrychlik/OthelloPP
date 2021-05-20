#include "Board.hpp"

#include <iostream>
#include <cstring>
#include <cmath>

#include <boost/test/unit_test.hpp>
//#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_addition)
{
  BOOST_CHECK( 2 + 2 == 4 );
}

BOOST_AUTO_TEST_CASE(test_board_print)
{
  Board b;
  std::cout << b;
  
}

BOOST_AUTO_TEST_CASE(test_board_print_big)
{
  Board b;
  b.print(std::cout, true);
}

BOOST_AUTO_TEST_CASE(test_board_print_small)
{
  Board b;
  b.print(std::cout, false);
}

BOOST_AUTO_TEST_CASE(test_board_move)
{
  Board b;
  auto move_bag = b.moves(true);
  for( auto& m : move_bag) {
    std::cout << std::get<0>(m) << "\n"
	      << std::get<1>(m) << "\n"
	      << std::get<2>(m) << "\n";
  }
}

