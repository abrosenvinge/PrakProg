CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o bilinear.o

# Out.txt: main
# 	./$< --test > $@

all: plane_plot.svg saddle_plot.svg gauss_plot.svg

saddle_plot.svg: bilinear_plot.gp saddle.data
	$(PLOT) $+

saddle.data: main Makefile
	./$< -n 100 -nx 10 -ny 10 --xbounds -2 2 --ybounds -1 1 -F saddle > $@

plane_plot.svg: bilinear_plot.gp plane.data
	$(PLOT) $+

plane.data: main Makefile
	./$< -n 100 -nx 10 -ny 5 --xbounds -1 1 --ybounds -1 1 -F plane > $@

gauss_plot.svg: bilinear_plot.gp gauss.data
	$(PLOT) $+

gauss.data: main Makefile
	./$< -n 100 -nx 10 -ny 15 --xbounds -5 5 --ybounds -5 5 -F gaussian > $@

main: $(objects)

main.o: main.cpp bilinear.hpp matrix.hpp

bilinear.o: bilinear.cpp bilinear.hpp matrix.hpp splines.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
