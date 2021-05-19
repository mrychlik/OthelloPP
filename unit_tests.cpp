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

BOOST_AUTO_TEST_CASE(test_board_move)
{
  Board b;
  auto c = b.move(true, 5, 4);
  std::cout << c;
  
}

