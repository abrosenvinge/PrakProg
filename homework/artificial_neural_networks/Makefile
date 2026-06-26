CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o ann.o ../minimization/minimization.o ../integration/integration.o

all: linear_plot.svg gauss_oscillation_plot.svg harm_oscillator_plot.svg

harm_oscillator_plot.svg: diff_eq_plot.gp harm_oscillator.data
	$(PLOT) $+

harm_oscillator.data: main Makefile
	./$< -n 5 -a 0 -b 3.1415 --harm_oscillator_diff_eq > $@

gauss_oscillation_plot.svg: ann_plot.gp gauss_oscillation.data
	$(PLOT) $+

gauss_oscillation.data: main Makefile
	./$< -n 10 -a -1 -b 1 --gauss_oscillation > $@

linear_plot.svg: ann_plot.gp linear.data
	$(PLOT) $+

linear.data: main Makefile
	./$< -n 10 -a -1 -b 1 --linear > $@

Out.txt: main
	./$< --test > $@

main: $(objects)

test: test.o ann.o ../minimization/minimization.o ../integration/integration.o

test.o: test.cpp ann.hpp ../minimization/minimization.hpp

main.o: main.cpp ann.hpp 

ann.o: ann.cpp ann.hpp ../lib/matrix.hpp

../minimization/minimization.o: ../minimization/minimization.hpp ../minimization/minimization.cpp

../integration/integration.o: ../integration/integration.hpp ../integration/integration.cpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data test.o
