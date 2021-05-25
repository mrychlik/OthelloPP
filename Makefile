# No threads yet!!!
#CXXFLAGS = -std=c++20 -O3 -ggdb3 -Wall -fomit-frame-pointer
CXXFLAGS = -std=c++20 -Og -O0 -ggdb3 -Wall
#CXXFLAGS = -std=c++20 -Ofast -ggdb3 -Wall
LDFLAGS  = -lm -lboost_unit_test_framework

SRCS = $(wildcard *.cpp)
PROGRAMS =  othello test_suite

all: $(PROGRAMS)

### A simple way to autogenerate header dependencies
depend: .depend

.depend: $(SRCS)
	rm -f "$@"
	$(CXX) $(CXXFLAGS) -MM $^ > "$@"

include .depend
### End of autogeneration of header dependencies

OTHELLO_OBJS = main.o Board.o GameTree.o MainLoop.o
othello: $(OTHELLO_OBJS)
	$(CXX) $(CXXFLAGS) $(OTHELLO_OBJS) -o $@ $(LDFLAGS)

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

