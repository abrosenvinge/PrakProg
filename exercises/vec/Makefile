CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o vec.o ../epsilon/approx.o

all : Out.txt
	cat Out.txt

Out.txt : main
	./$< > $@

main: $(objects)

main.o: main.cpp vec.hpp

vec.o: vec.cpp vec.hpp ../epsilon/approx.hpp

../epsilon/approx.o: ../epsilon/approx.hpp

clean:
	$(RM) main Out.txt $(objects)
