CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o vec.o

Out.txt : main
	# ./main > Out.txt
	./$< > $@

main: $(objects)

main.o: main.cpp vec.hpp

vec.o: vec.cpp vec.hpp

clean:
	$(RM) main Out.txt $(objects)
