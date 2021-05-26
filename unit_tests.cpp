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
#include "SimpleStaticEvaluator.hpp"
#include "CornerStaticEvaluator.hpp"
#include "MainLoop.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>

#include <boost/test/unit_test.hpp>
//#include <boost/test/auto_unit_test.hpp>
#include <boost/format.hpp>

BOOST_AUTO_TEST_CASE(addition)
{
  BOOST_CHECK( 2 + 2 == 4 );
}

BOOST_AUTO_TEST_CASE(board_print)
{
  Board b;
  std::cout << b;
  
}

BOOST_AUTO_TEST_CASE(board_print_big)
{
  Board b;
  b.print(std::cout, true);
}

BOOST_AUTO_TEST_CASE(board_print_small)
{
  Board b;
  b.print(std::cout, false);
}

BOOST_AUTO_TEST_CASE(board_move)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves(Board::WHITE);
  for( auto& m : move_bag) {
    std::cout << std::get<0>(m) << "\n"
	      << std::get<1>(m) << "\n"
	      << std::get<2>(m) << "\n";
  }
}

BOOST_AUTO_TEST_CASE(board_move_fancy)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves(Board::WHITE);
  for( auto& m : move_bag) {
    auto [x, y, board] = m;
    std::cout << "x: " << x << ", "
	      << "y: " << y << "\n"
	      << board << "\n";
  }
}

BOOST_AUTO_TEST_CASE(board_move_text)
{
  // TODO: Use several boards with known
  // move bag as test cases

  Board b;
  auto move_bag = b.moves(Board::WHITE);
  for( auto& m : move_bag) {
    auto [x, y, board] = m;
    std::cout << "x: " << x << ", "
	      << "y: " << y << "\n";
  }
}

BOOST_AUTO_TEST_CASE(board_size)
{
  Board b;
  std::cout << "Board size: " << sizeof(b) << std::endl;
}

BOOST_AUTO_TEST_CASE(tree_node_size)
{
  TreeNode tree;
  std::cout << "TreeNode size: " << sizeof(tree) << std::endl;
}

BOOST_AUTO_TEST_CASE(tree_minmax_and_print)
{
  TreeNode root;
  int depth = 5;
  SimpleStaticEvaluator evaluator;
  root.minmax(evaluator, depth);
  std::cout << "\nDepth : " << depth << "\n"
	    << root
	    << std::endl;
}

BOOST_AUTO_TEST_CASE(tree_minmax)
{
  TreeNode root;
  int depth = 16;
  SimpleStaticEvaluator evaluator;
  std::cout << "\nDepth: " << depth
	    << "\nMinMax value: " << root.minmax(evaluator, depth)
    	    << std::endl;
}

BOOST_AUTO_TEST_CASE(tree_minmax_corners)
{
  TreeNode root;
  int depth = 16;
  CornerStaticEvaluator evaluator;
  std::cout << "\nDepth: " << depth
	    << "\nMinMax value: " << root.minmax(evaluator, depth)
    	    << std::endl;
}


/** 
 * Compute node count for given board size.
 * 
 * @param w  Board width
 * @param h  Board height
 * @param max_depth Maximum depth
 */
static void node_count(unsigned w, unsigned h, int max_depth)
{
  std::cout << boost::format("Node count for board size %u x %u, max. depth: %u\n") % w % h % max_depth;
  Board::setW(w);
  Board::setH(h);  
  TreeNode root;

  std::cout << boost::format("%5s %10s %10s %10s\n") % "Depth" % "Node Count" % "Fanout" % "Bar plot";

  for(auto depth = 0; depth < max_depth; ++depth) {
    try {
      auto count = root.nodeCount(depth);
      auto fanout = (::log(count)/depth/log(2));
      int bar_len = ::round(10*fanout);
      std::cout << boost::format("%5d %10d %10g ") % depth % count % fanout
		<< '|' << std::setfill ('=') << std::setw (bar_len) << '|'
		<< std::endl;

    } catch(std::bad_alloc& e) {
      std::cerr << e.what();
    } catch(...) {
      std::cerr << "Something bad happened at depth: " << depth << "\n";
    }
  }
}

BOOST_AUTO_TEST_CASE(tree_node_count)
{
  node_count(8, 8, 11);
}

BOOST_AUTO_TEST_CASE(tree_node_count_4x4)
{
  node_count(4, 4, 20);
}

BOOST_AUTO_TEST_CASE(tree_node_count_6x6)
{
  node_count(6, 6, 10);
}

BOOST_AUTO_TEST_CASE(main_loop_test)
{
  max_depth[Board::WHITE] = 12;
  max_depth[Board::BLACK] = 12;  
  num_games = 1;
  main_loop();
}

BOOST_AUTO_TEST_CASE(solution_4x4)
{
  std::cout << "Solves the 4x4 case" << std::endl;
  Board::setH(4);
  Board::setW(4);
  max_depth[Board::WHITE] = 18;
  max_depth[Board::BLACK] = 18;  
  num_games = 1;
  main_loop();
}
