CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o bilinear.o

# Out.txt: main
# 	./$< --test > $@

plane_plot.svg: bilinear_plot.gp plane.data
	$(PLOT) $+

plane.data: main
	./$< > $@

main: $(objects)

main.o: main.cpp bilinear.hpp matrix.hpp

bilinear.o: bilinear.cpp bilinear.hpp matrix.hpp splines.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
