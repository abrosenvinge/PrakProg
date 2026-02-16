CXXFLAGS += --std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o

fig1.svg: fig1.gpi
	gnuplot fig1.gpi

erf.data: main Makefile
	./main --xmin -2 --xmax 2 --dx 0.25 1>$@ 2>log.txt

main: main.o

main.o: main.cpp
