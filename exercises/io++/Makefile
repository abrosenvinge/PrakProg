CXX = clang++
CXXFLAGS = -std=c++23 -O -Wall -Wextra
LDLIBS += -lstdc++ -lm

objects = main.o input.o

all : Out.txt
	cat Out.txt

Out.txt : main cmd-input.txt stdin-input.txt file-input.txt
	./main -i file-input.txt -o output.txt $$(cat cmd-input.txt) < stdin-input.txt > $@

main: main.o

main.o: main.cpp

cmd-input.txt:
	> $@
	for i in $$(seq 0 10); do echo "-n $$i" >> $@; done

stdin-input.txt:
	seq 0 10 > $@

file-input.txt:
	seq 0 10 > $@

clean:
	$(RM) main Out.txt $(objects) cmd-input.txt stdin-input.txt file-input.txt output.txt
