CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o bilinear.o

# Out.txt: main
# 	./$< --test > $@

all: interp_plots timing_plot.svg Out.txt

interp_plots: saddle_plot.svg plane_plot.svg gauss_cos_plot.svg gauss_plot.svg wave_plot.svg

timing_plot.svg: timing_plot.gp timing.data
	$(PLOT) $+

timing.data: main Makefile
	>$@
	# note that time outputs to stderr so redirect it to stdout with 2>&1
	for i in $$(seq 100 400 2000); do \
		echo $$i;\
		echo\
			$$i \
			$$($(TIME) --format "%e" ./$< -n $$i -nx 20 -ny 20 --xbounds -1 1 --ybounds -1 1 -F saddle --plot --nooutput --mode normal 2>&1) \
			$$($(TIME) --format "%e" ./$< -n $$i -nx 20 -ny 20 --xbounds -1 1 --ybounds -1 1 -F saddle --plot --nooutput --mode grid 2>&1) \
		>> $@ \
	; done

wave_plot.svg: bilinear_plot.gp wave.data
	$(PLOT) $+

wave.data: main Makefile
	./$< -n 200 -nx 40 -ny 40 --xbounds -20 20 --ybounds -20 20 -F wave --plot --output > $@

gauss_cos_plot.svg: bilinear_plot.gp gauss_cos.data
	$(PLOT) $+

gauss_cos.data: main Makefile
	./$< -n 200 -nx 20 -ny 30 --xbounds -2 2 --ybounds -1 1 -F gauss_cos --plot --output --mode grid > $@

saddle_plot.svg: bilinear_plot.gp saddle.data
	$(PLOT) $+

saddle.data: main Makefile
	./$< -n 100 -nx 10 -ny 10 --xbounds -2 2 --ybounds -1 1 -F saddle --plot --output > $@

plane_plot.svg: bilinear_plot.gp plane.data
	$(PLOT) $+

plane.data: main Makefile
	./$< -n 100 -nx 10 -ny 5 --xbounds -1 1 --ybounds -1 1 -F plane --plot --output --mode grid > $@

gauss_plot.svg: bilinear_plot.gp gauss.data
	$(PLOT) $+

gauss.data: main Makefile
	./$< -n 100 -nx 15 -ny 15 --xbounds -5 5 --ybounds -5 5 -F gaussian --xspacing logarithmic --yspacing logarithmic --plot --output > $@

Out.txt: main
	echo > $@
	./$< -n 100 -nx 10 -ny 5 --xbounds -1 1 --ybounds -1 1 -F plane --test >> $@
	./$< -n 100 -nx 10 -ny 10 --xbounds -2 2 --ybounds -1 1 -F saddle --test >> $@
	./$< -n 100 -nx 15 -ny 15 --xbounds -5 5 --ybounds -5 5 -F gaussian --xspacing logarithmic --yspacing logarithmic --test >> $@
	./$< -n 200 -nx 20 -ny 30 --xbounds -2 2 --ybounds -1 1 -F gauss_cos --test >> $@
	./$< -n 200 -nx 40 -ny 40 --xbounds -20 20 --ybounds -20 20 -F wave --test >> $@

main: $(objects)

main.o: main.cpp bilinear.hpp matrix.hpp

bilinear.o: bilinear.cpp bilinear.hpp matrix.hpp splines.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
