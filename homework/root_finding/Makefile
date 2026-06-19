CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
LDLIBS += -lstdc++ -lm
TIME = /usr/bin/time

PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o root_finding.o ../ODE/ode.o

# Out.txt: main
# 	./$< --pprint > $@

main: $(objects)

main.o: main.cpp root_finding.hpp 

root_finding.o: root_finding.cpp root_finding.hpp ../lib/matrix.hpp ../ODE/ode.hpp

../ODE/ode.o: ../ODE/ode.cpp ../ODE/ode.hpp

clean:
	$(RM) main Out.txt $(objects) *.svg *.data
