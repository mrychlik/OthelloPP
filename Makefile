CXXFLAGS = -std=c++20 -Ofast -ggdb3 -pthread -Wall
LDFLAGS  = -lm -lboost_unit_test_framework

PROGRAMS =  main test_suite

all: $(PROGRAMS)

main.o: Board.hpp GameTree.o
Board.o: Board.hpp
GameTree.o: GameTree.hpp Board.hpp

OBJS = main.o Board.o GameTree.o
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

UNIT_OBJS = unit_tests.o testlib.o Board.o GameTree.o
test_suite: $(UNIT_OBJS)
	$(CXX) $(CXXFLAGS) $(UNIT_OBJS) -o $@ $(LDFLAGS)

check: test_suite
	./test_suite

clean: 
	-@rm *.o
	-@rm $(PROGRAMS)

doc: Doxyfile
	doxygen $<

