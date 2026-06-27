CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra -O
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o test.o

all: plot.svg Out.txt test.log

plot.svg: plot.gp out.times.data
	$(PLOT) $+ 2> plot.log

Out.txt: main
	./$< -M 8 -N 5 > $@

out.times.data : main Makefile
	>$@
	>log.main
	for N in $$(seq 100 20 400); do \
		$(TIME) --format "$$N %e" --append --output $@ \
			./$< -N $$N -M $$N --time 1> /dev/null 2>> log.main; \
	done 

main: main.o

main.o: main.cpp qr.hpp ../lib/matrix.hpp

test.log: test
	./test 1> test.log

test: test.o

test.o: test.cpp qr.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) test.log log.main *.data *.svg
