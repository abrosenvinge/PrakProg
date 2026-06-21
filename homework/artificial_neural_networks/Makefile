CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o ann.o

all: linear_plot.svg damped_oscillation_plot.svg

damped_oscillation_plot.svg: ann_plot.gp damped_oscillation.data
	$(PLOT) $+

damped_oscillation.data: main Makefile
	./$< -n 10 -a -1 -b 1 --damped_oscillation > $@

linear_plot.svg: ann_plot.gp linear.data
	$(PLOT) $+

linear.data: main Makefile
	./$< -n 10 -a -1 -b 1 --linear > $@

Out.txt: main
	./$< --test > $@

main: $(objects)

main.o: main.cpp ann.hpp 

ann.o: ann.cpp ann.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
