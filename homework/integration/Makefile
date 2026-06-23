CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o integration.o

all: Out.txt erf_error_plot.svg erf_plot.svg

erf_error_plot.svg: erf_error_plot.gp erf_error.data
	$(PLOT) $+

erf_error.data: main Makefile
	./$< --erf_error 1 0.00000000001 100 > $@

erf_plot.svg: erf_plot.gp erf_plot.data
	$(PLOT) $+

erf_plot.data: main Makefile
	./$< --erf -5 5 100 > $@

Out.txt: main cc_test_print.sh cc_test.py Makefile
	echo > $@
	sh ./cc_test_print.sh 'sqrt(x)' 0 1 0.666666666 >> $@
	echo >> $@
	sh ./cc_test_print.sh '1/sqrt(x)' 0 1 2 >> $@
	echo >> $@
	sh ./cc_test_print.sh 'sqrt(1-x*x)' 0 1 0.78540 >> $@
	echo >> $@
	sh ./cc_test_print.sh 'ln(x)/sqrt(x)' 0 1 -4 >> $@
	echo >> $@
	sh ./cc_test_print.sh 'exp(-x*x)' -INF INF 1.77245 >> $@
	echo >> $@
	sh ./cc_test_print.sh 'exp(-x)' 0 INF 1 >> $@

main: main.o integration.o

main.o: main.cpp integration.hpp

integration.o: integration.hpp integration.cpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
