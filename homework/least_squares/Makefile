CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra -O
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o

plot.png: plot.gp params.txt
	$(PLOT) $+

# grouped target since main produces both
Out.txt params.txt &: main ThX_decay.data
	cat ThX_decay.data | ./$< 2> Out.txt 1> params.txt

main: main.o

main.o: main.cpp least_squares.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) *.png
