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
