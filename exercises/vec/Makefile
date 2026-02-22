CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o approx.o

all : Out.txt
	cat Out.txt

Out.txt : main
	./$< > $@

main: $(objects)

main.o: main.cpp vec.hpp

# vec.o: vec.hpp ../epsilon/approx.hpp

approx.o: ../epsilon/approx.cpp ../epsilon/approx.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) main Out.txt $(objects)
