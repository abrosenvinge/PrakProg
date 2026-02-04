CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o sfuns.o

all : Out.txt
	cat Out.txt

Out.txt : main
	./$< > $@

main: $(objects)

main.o: main.cpp sfuns.hpp

sfuns.o: sfuns.cpp sfuns.hpp

clean:
	$(RM) main Out.txt $(objects)
