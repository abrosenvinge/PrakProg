CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o monte_carlo.o

all: Out.txt scaling_plot.svg

Out.txt: main
	./$< --pprint > $@

# this may take a while with these numbers
scaling.data: main Makefile
	./$< --scaling 10000 1000000 10000 > $@

scaling_plot.svg: scaling_plot.gp scaling.data
	$(PLOT) $+ 2> fit.log

main: $(objects)

main.o: main.cpp monte_carlo.hpp

monte_carlo.o: monte_carlo.cpp monte_carlo.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
