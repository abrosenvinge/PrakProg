CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra -O
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o test.o

all: wavefunc_plot.png rmax_plot.png dr_plot.png Out.txt

timing_plot.png: timing_plot.gp timing_plot.data
	$(PLOT) $+

timing_plot.data: main Makefile
	>$@
	# note that time outputs to stderr so redirect it to stdout with 2>&1
	for i in $$(seq 80 10 300); do \
		echo\
			$$i \
			$$($(TIME) --format "%e" ./$< --time -N $$i 2>&1) \
			$$($(TIME) --format "%e" ./$< --time -N $$i --use_unoptimized 2>&1) \
		>> $@ \
	; done

wavefunc_plot.png: wavefunc_plot.gp wavefunc_plot.data
	$(PLOT) $+

wavefunc_plot.data: main Makefile
	./$< --dr 0.05 --rmax 10 --nwavefuncs 5 > $@

rmax_plot.png: rmax_plot.gp rmax_plot.data
	$(PLOT) $+

rmax_plot.data: main Makefile
	> $@
	# LC_ALL=C because decimals otherwise use "," on my machine
	for i in $$(LC_ALL=C seq 1 1 15); do \
		./$< --rmax $$i --dr 0.1 --e0 >> $@ \
	; done

dr_plot.png: dr_plot.gp dr_plot.data
	$(PLOT) $+

dr_plot.data: main Makefile
	> $@
	for i in $$(LC_ALL=C seq 0.3 -0.01 0.05); do \
		./$< --dr $$i --rmax 10 --e0 >> $@ \
	; done

Out.txt: main
	./$< --test

main: main.o

main.o: main.cpp evd.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) *.data *.png
