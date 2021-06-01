/**
 * @file   unit_tests_main_loop.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Fri May 21 10:11:39 2021
 * 
 * @brief  Unit tests according to the Boost unit testing framework
 * 
 * 
 */

#include "MainLoop.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>

#include <boost/test/unit_test.hpp>
//#include <boost/test/auto_unit_test.hpp>
#include <boost/format.hpp>


BOOST_AUTO_TEST_CASE(main_loop_test)
{
  MainLoop::getInstance()
    .setMaxDepth(BoardTraits::WHITE, 12)
    .setMaxDepth(BoardTraits::BLACK, 12)  
    .setNumGames(1)
    .run();
}

BOOST_AUTO_TEST_CASE(solution_4x4)
{
  std::cout << "Solves the 4x4 case" << std::endl;
  MainLoop::getInstance()
    .setBoardWidth(4)
    .setBoardHeight(4)
    .setMaxDepth(BoardTraits::WHITE, 18)
    .setMaxDepth(BoardTraits::BLACK, 18)  
    .setNumGames(1)
    .run();
}

BOOST_AUTO_TEST_CASE(solution_4x6)
{
  std::cout << "Solves the 4x6 case" << std::endl;
  MainLoop::getInstance()
    .setBoardWidth(4)
    .setBoardHeight(6)
    .setMaxDepth(BoardTraits::WHITE, 15)
    .setMaxDepth(BoardTraits::BLACK, 15)  
    .setNumGames(1)
    .run();
}

BOOST_AUTO_TEST_CASE(solution_6x4)
{
  std::cout << "Solves the 6x4 case" << std::endl;
  MainLoop::getInstance()
    .setBoardWidth(6)
    .setBoardHeight(4)
    .setMaxDepth(BoardTraits::WHITE, 15)
    .setMaxDepth(BoardTraits::BLACK, 15)  
    .setNumGames(1)
    .run();
}
