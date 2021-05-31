# OthelloPP
A C++ implementation of the game *Othello*.

## The origin
The project code implementing computations related to the
game Othello was derived from the code in the project:
[TCCQ/Othello](https://github.com/TCCQ/Othello).

Most of the design of the C++ code is different, however.
Therefore, the code should not be considered a fork of
the original project.

## Highlights
The binary program othello plays a game of Othello using the minmax
algorithm with alpha/beta pruning. The program can use any M-by-N board
where M, N are in the set {4, 6, 8}. For each of these sizes 
the initial position of the 4 pieces is at the center of the board.

The game can be human against human, human against computer or
computer against computer (automatic play).

The program renders a *strong solution* of the game for 4x4, 4x6 and
6x4 game, demonstrating that white wins by moving optimally. The
program runs out of memory for 6x6 and above boards on a computer with
16GB of memory (+8GB swap space) in about 1.5 minutes.
The page [Computer Othello](https://en.wikipedia.org/wiki/Computer_Othello)
reports that for the 6x6 board the game of Othello was strongly solved,
and that white wins with optimal game. The result required
100 hours of distributed computation, according to this source.


## The original author's README

This is a rewrite of my original java othello playing script.

This branch is written in C++ and is more memory efficient. It uses
similar processes, however, so it may not be more efficient
computationally. It is written to maximize the use of bitlevel
opertations and minimize the size of each object to enable God's
Algorithm style gameplay. My intent is explore as many possible
gameplay branches (possible moves of the cpu and of the player)
according to a breadth first search, and use a hueristic to determine
which is the most desirable, and then guide the game along that branch
when it is the cpu's turn.

Usage: othe [-c, -q n]
-c: compact mode, smaller, more basic terminal output
-q: requires integer n to immediately follow. sets the depth of tree exploration. defaults to 4 if not specified. 

Tentative and inaccurate analysis shows that CPU usage is quite high
and extremely liable to spike, as all computation happens during the
computer's turn, largely as fast as possible, as time spent copying
memory etc. is minimized. Depths above 5 are enough to give a 2.6 GHz
CPU a 99% usage rating for several seconds. Thought will be given to
how to balance memory usage and CPU load. More analysis will inform
that choice. Adding a argument switch for CPU vs RAM priority will be
considered.

Performance analysis was performed using gcov and gprof (requires
compiling and linking with "-pg --coverage"). After consiterations,
the size of one board instance was increased by two bytes (16 to 18
bytes) in order to allow for one player to take multiple turns in a
row, and to cache score and tile number. Both score() and tileNum()
were contributing roughly 20% of the total computation time, and are
now performed in constant, negligible time.

Feel free to steal this and credit me.

This branch is much more likely to be updated than main.

--Thomas Ulmer
