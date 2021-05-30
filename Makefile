# No threads yet!!!
CXXFLAGS = -std=c++2a
CXXFLAGS += -ggdb3
#CXXFLAGS += -O3 -Wall -fomit-frame-pointer
#CXXFLAGS += -Og -O0 -Wall
#CXXFLAGS += -Ofast -Og -Wall
#CXXFLAGS += -Ofast -Og -Wall -fomit-frame-pointer
CXXFLAGS += -Ofast -Wall -msse4 -DNDEBUG=1 -fomit-frame-pointer

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

OTHELLO_OBJS = main.o Board.o TreeNode.o MainLoop.o
othello: $(OTHELLO_OBJS)
	$(CXX) $(CXXFLAGS) $(OTHELLO_OBJS) -o $@ $(LDFLAGS)

UNIT_OBJS = unit_tests_board.o unit_tests_tree.o unit_tests_main_loop.o testlib.o Board.o MainLoop.o TreeNode.o
test_suite: $(UNIT_OBJS)
	$(CXX) $(CXXFLAGS) $(UNIT_OBJS) -o $@ $(LDFLAGS)

check: test_suite
	./test_suite

check_leaks:
	valgrind -s --leak-check=full --show-leak-kinds=all  ./othello -D 0 -r 6 -c 6 -n 1 > /dev/null

clean: 
	-@rm *.o
	-@rm $(PROGRAMS)

doc: Doxyfile
	doxygen $<

