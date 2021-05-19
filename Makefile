CXXFLAGS = -std=c++20 -O0 -ggdb3 -pthread
LDFLAGS  = -lm -lboost_unit_test_framework

all: main test_suite

main.o: Board.hpp
Board.o: Board.hpp

OBJS = main.o Board.o
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

UNIT_OBJS = unit_tests.o testlib.o
test_suite: $(UNIT_OBJS)
	$(CXX) $(CXXFLAGS) $(UNIT_OBJS) -o $@ $(LDFLAGS)

clean: 
	-rm *.o
	-rm $(PROGRAMS)

doc: Doxyfile
	doxygen $<

