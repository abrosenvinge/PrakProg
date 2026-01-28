# CXX = clang++
CXXFLAGS = -std=c++23 -O
LDLIBS = -lstdc++ -lm

objects = main.o hello.o

Out.txt : main
	./main > Out.txt


# $(CXX) $(objects) -o main $(LDLIBS)
#
# linked as $(CC) $(LDFLAGS) $(objects) $(LOADLIBES) $(LDLIBS)
main: $(objects)

# compiled as $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c
# from main.cpp
main.o: hello.hpp

hello.o: hello.hpp

clean:
	$(RM) main Out.txt $(objects)

test:
	echo $(CXX)
