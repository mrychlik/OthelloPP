CXXFLAGS = -std=c++20 -O0 -ggdb3 -pthread
LDFLAGS  = -lm -lboost_unit_test_framework

all: main test_suite

main.o: Board.hpp
Board.o: Board.hpp
GameTree.o: GameTree.hpp

OBJS = main.o Board.o GameTree.o
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

UNIT_OBJS = unit_tests.o testlib.o Board.o
test_suite: $(UNIT_OBJS)
	$(CXX) $(CXXFLAGS) $(UNIT_OBJS) -o $@ $(LDFLAGS)

check: test_suite
	./test_suite

clean: 
	-rm *.o
	-rm $(PROGRAMS)

doc: Doxyfile
	doxygen $<

