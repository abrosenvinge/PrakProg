CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o integration.o

Out.txt: main
	./$< > $@

main: main.o integration.o

main.o: main.cpp integration.hpp

integration.o: integration.hpp

clean:
	$(RM) main Out.txt $(objects) *.png *.data
