CXX = g++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o ode.o

three_body_plot.png: three_body_plot.gp three_body_plot.data
	$(PLOT) $+

three_body_plot.data: main Makefile
	./$< --mode three_body_problem --start 0 --end 9.96150788812 --eps 0.00001 --acc 0.00001 > $@

rel_orbit_plot.png: rel_orbit_plot.gp rel_orbit_plot.data
	$(PLOT) $+

rel_orbit_plot.data: main Makefile
	> $@
	./$< --mode rel_orbit --start 0 --end 314.159265359 --y0 1 --y0 0 --acc 0.0001 --eps 0.0001 --max_step 0.01 >> $@
	./$< --mode rel_orbit --start 0 --end 314.159265359 --y0 1 --y0 -0.5 --acc 0.0000001 --eps 0.0000001 >> $@
	./$< --mode rel_orbit --start 0 --end 408.407044967 --y0 1 --y0 -0.5 --epsilon 0.01 --acc 0.0000001 --eps 0.0000001 >> $@

lotka_volterra_plot.png: lotka_volterra_plot.gp lotka_volterra_plot.data
	$(PLOT) $+

lotka_volterra_plot.data: main Makefile
	./$< --mode lotka_volterra --start 0 --end 15 --acc 0.0001 --eps 0.0001 > $@

shm_plot.png: shm_plot.gp shm_plot.data
	$(PLOT) $+

shm_plot.data: main Makefile
	./$< --mode shm --start 0 --end 2 > $@

main: main.o ode.o

main.o: main.cpp ode.hpp ../lib/matrix.hpp

ode.o: ode.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) *.png *.data
