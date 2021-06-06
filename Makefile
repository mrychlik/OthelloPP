# No threads yet!!!
CXXFLAGS = -std=c++2a
CXXFLAGS += -ggdb3
CXXFLAGS += -Wall -Wextra
#CXXFLAGS += -DUSE_LUT=1	   #Use lookup table
#CXXFLAGS += -O3-fomit-frame-pointer
#CXXFLAGS += -Og -O0
#CXXFLAGS += -Ofast -Og
#CXXFLAGS += -Ofast -Og -fomit-frame-pointer
CXXFLAGS += -Ofast -msse4 -DNDEBUG=1 -fomit-frame-pointer

LDFLAGS  = -lm -lboost_unit_test_framework

SRCS = $(wildcard *.cpp)
PROGRAMS =  othello test_suite

all: $(PROGRAMS)

### A simple way to autogenerate header dependencies
depend: .depend

.depend: $(SRCS)
	-@rm -f "$@"
	$(CXX) $(CXXFLAGS) -MM $^ > "$@"

include .depend
### End of autogeneration of header dependencies

othello: main.o Board.o TreeNode.o MainLoop.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test_suite: unit_tests_board.o unit_tests_tree.o unit_tests_main_loop.o testlib.o Board.o MainLoop.o TreeNode.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

check: test_suite
	./test_suite

check-for-leaks: check-for-leaks-random-moves check-for-leaks-alphabeta check-for-leaks-full-minmax

check-for-leaks-random-moves: othello
	valgrind -s --leak-check=full --show-leak-kinds=all  ./othello -D 0 -r 6 -c 6 -n 1 > /dev/null 

check-for-leaks-alphabeta: othello
	valgrind -s --leak-check=full --show-leak-kinds=all  ./othello -D 6 -r 6 -c 6 -n 1 > /dev/null 

check-for-leaks-full-minmax: othello
	valgrind -s --leak-check=full --show-leak-kinds=all  ./othello -D 1000 -A 1 -r 4 -c 4 -n 1 > /dev/null 

# Using prof, requires Linux kernel 2.6+
# Must be run in a terminal capable of terminal graphics
profile: othello
	perf record -g ./othello -D 6 -r 8 -c 8 -n 1 > /dev/null
	perf report

clean-binaries: 
	-@rm *.o
	-@rm $(PROGRAMS)

clean-doc:
	-@rm ./docs/html/*
	-@rm ./docs/html/search/*
	-@rm ./docs/latex/*


clean: clean-binaries

doc: Doxyfile
	doxygen $<
	(cd ./docs/latex; make )
