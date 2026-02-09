CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o vec.o

all : Out.txt
	cat Out.txt

Out.txt : main
	./$< > $@

main: $(objects)

main.o: main.cpp vec.hpp

vec.o: vec.cpp vec.hpp

clean:
	$(RM) main Out.txt $(objects)
