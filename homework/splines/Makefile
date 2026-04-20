CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra -O
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o

all: exp_plot.png quadratic_plot.png sin_plot.png

exp_plot.png: plot.gp exp_plot.data
	$(PLOT) $+ "exp"

exp_plot.data: main Makefile
	./$< --start -5 --end 5 -f exp --n_points 10 --n_interp_points 100 > $@

quadratic_plot.png: plot.gp quadratic_plot.data
	$(PLOT) $+ "quadratic"

quadratic_plot.data: main Makefile
	./$< --start -5 --end 5 -f quadratic --n_points 10 --n_interp_points 100 > $@

sin_plot.png: plot.gp sin_plot.data
	$(PLOT) $+ "sin"

sin_plot.data: main Makefile
	./$< -f sin --n_points 20 --n_interp_points 500 > $@

main: main.o

main.o: main.cpp splines.hpp

clean:
	$(RM) main Out.txt $(objects) *.png
