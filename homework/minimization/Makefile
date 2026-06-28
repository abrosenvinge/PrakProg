CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o minimization.o ../ODE/ode.o

all: higgs_plot.svg

higgs_plot.svg: higgs_plot.gp higgs_fit.data higgs_data.txt # I realize higgs_data.txt is already a dependency of higgs_fit.data, but have included it here so $+ can be passed to the plot.
	$(PLOT) $+

Out.txt higgs_fit.data &: main higgs_data.txt Makefile
	./$< --test > Out.txt
	echo >> Out.txt
	./$< --plot_points 1000 --acc 0.0001 --p0 126 2. 9. --higgs < higgs_data.txt 1> higgs_fit.data 2>> Out.txt

main: $(objects)

main.o: main.cpp minimization.hpp 

minimization.o: minimization.cpp minimization.hpp ../lib/matrix.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
