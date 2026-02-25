CXXFLAGS = -std=c++23 -Wall -O
LDLIBS = -lstdc++ -lm

PLOT = gnuplot -e 'out = "$@"' -c $+

all: gamma_plot.png erf_plot.png lngamma.png

main: main.cpp

gamma_gen.data: main Makefile
	./$< --gamma --xmin -5 --xmax 5 --dx 0.001 > $@

gamma_plot.png: gamma_plot.gp gamma.data gamma_gen.data
	$(PLOT)

erf_gen.data: main Makefile
	./$< --erf --xmin 0 --xmax 3.5 > $@

erf_plot.png: erf_plot.gp erf.data erf_gen.data
	$(PLOT)

lngamma_gen.data: main Makefile
	./$< --lngamma --xmin 0 --xmax 20 > $@

lngamma.png: lngamma_plot.gp lngamma.data lngamma_gen.data
	$(PLOT)

clean:
	$(RM) *.png *gen.data main

