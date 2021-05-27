/**
 * @file   unit_tests_tree.cpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Fri May 21 10:11:39 2021
 * 
 * @brief  Unit tests according to the Boost unit testing framework
 * 
 * 
 */

#include "GameTree.hpp"
#include "SimpleStaticEvaluator.hpp"
#include "CornerStaticEvaluator.hpp"
#include "StaticEvaluator.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>

#include <boost/test/unit_test.hpp>
//#include <boost/test/auto_unit_test.hpp>
#include <boost/format.hpp>


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
  int depth = 15;
  SimpleStaticEvaluator evaluator;
  std::cout << "\nDepth: " << depth
	    << "\nMinMax value: " << root.minmax(evaluator, depth)
    	    << std::endl;
}

BOOST_AUTO_TEST_CASE(tree_minmax_corners)
{
  TreeNode root;
  int depth = 15;
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

BOOST_AUTO_TEST_CASE(tree_node_count_6x4)
{
  node_count(6, 4, 12);
}

BOOST_AUTO_TEST_CASE(tree_node_count_4x6)
{
  node_count(4, 6, 12);
}

BOOST_AUTO_TEST_CASE(tree_copy_assign_throw)
{
  // Ensure that we can copy-assign with rv being a child only

  TreeNode t1, t2;		// Two unrelated nodes

  // Do some computations to cause expansion
  SimpleStaticEvaluator evaluator;
  int depth = 3;
  t1.minmax(evaluator, depth);
  t2.minmax(evaluator, depth);

  //BOOST_CHECK_THROW (expression, an_exception_type);
  BOOST_REQUIRE_THROW( t1 = t2, std::logic_error );
}

BOOST_AUTO_TEST_CASE(tree_copy_assign_nothrow)
{
  // Ensure that we can copy-assign from a child
  TreeNode t1;		
  const TreeNode* t2 = *t1.children().begin();

  //BOOST_CHECK_NO_THROW (expression)
  BOOST_REQUIRE_NO_THROW( t1 = *t2 );
}
