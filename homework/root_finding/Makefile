CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o root_finding.o ../ODE/ode.o

all: Out.txt wavefunc_plot.svg scaling_plot.svg

wavefunc_plot.svg: wavefunc_plot.gp wavefunc.data
	$(PLOT) $+

# This can take a little bit to make with the numbers I have set
scaling_plot.svg: scaling_plot.gp rmax.data rmin.data acc.data eps.data
	$(PLOT) $+

rmax.data: main Makefile
	> $@
	for r in $$(seq 2 0.05 25); do \
		printf "%s %s\n" "$$r" "$$(./$< --rmin 0.0000001 --rmax $$r --acc 0.0000001 --eps 0.0000001 --E0 -0.6 --shoot)" >> $@ \
	; done

rmin.data: main Makefile
	> $@
	# LC_ALL=C because decimals otherwise use "," on my machine
	for r in $$(LC_ALL=C seq 0.3 -0.0005 0.0005); do \
		printf "%s %s\n" "$$r" "$$(./$< --rmin $$r --rmax 15 --acc 0.0000001 --eps 0.0000001 --E0 -0.6 --shoot)" >> $@ \
	; done

acc.data: main Makefile
	> $@
	# LC_ALL=C because decimals otherwise use "," on my machine
	for a in $$(LC_ALL=C seq 0.1 -0.00005 0.00005); do \
		printf "%s %s\n" "$$a" "$$(./$< --rmin 0.0000001 --rmax 15 --acc $$a --eps 0.00001 --E0 -0.6 --shoot)" >> $@ \
	; done

eps.data: main Makefile
	> $@
	# LC_ALL=C because decimals otherwise use "," on my machine
	for e in $$(LC_ALL=C seq 0.1 -0.00005 0.00005); do \
		printf "%s %s\n" "$$e" "$$(./$< --rmin 0.0000001 --rmax 15 --acc 0.00001 --eps $$e --E0 -0.6 --shoot)" >> $@ \
	; done

RMAX = 15
wavefunc.data: main Makefile
	./$< --rmin 0.0000001 --rmax $(RMAX) --steps 1000 --actual_wavefunc > $@
	for r in $$(seq 3 2 $(RMAX)); do \
		printf "\n\nrmax=%s\n" "$$r" >> $@; \
		./$< --rmin 0.0000001 --rmax $$r --acc 0.0000001 --eps 0.0000001 --wavefunc >> $@ \
	; done

Out.txt: main
	./$< --test > $@

main: $(objects)

main.o: main.cpp root_finding.hpp 

root_finding.o: root_finding.cpp root_finding.hpp ../lib/matrix.hpp ../ODE/ode.hpp

../ODE/ode.o: ../ODE/ode.cpp ../ODE/ode.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
