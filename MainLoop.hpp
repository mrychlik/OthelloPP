/**
 * @file   MainLoop.hpp
 * @author Marek Rychlik <marek@cannonball.lan>
 * @date   Mon May 24 18:14:26 2021
 * 
 * @brief  Declarations for main loop implementation
 * 
 * 
 */

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include <iosfwd>
#include "StaticEvaluator.hpp"

extern int  max_depth[2];
extern bool humanPlayer[2];
extern int num_games;
extern int computer_delay;

extern int main_loop(const StaticEvaluatorTable& evaluator, std::istream& ins, std::ostream& os, std::ostream& logs);
extern int main_loop(const StaticEvaluatorTable& evaluator);
extern int main_loop();

#endif /* MAIN_LOOP */
