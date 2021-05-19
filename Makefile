ARGS = -std=c++11 -g -pthread -Wall -Wextra

all: othe
clean:
	rm *.o othe 

Oboard.o : Oboard.cc Oboard.hh 
	g++ ${ARGS} -c Oboard.cc

main.o : Oboard.hh main.cc
	g++ ${ARGS} -c main.cc

othe : Oboard.o main.o
	g++ ${ARGS} Oboard.o main.o -o othe
