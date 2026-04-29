CXXFLAGS = -std=c++23 -O
LDLIBS = -lstdc++ -lm -lpthread

TIME = /usr/bin/time
PLOT = gnuplot -e "set output \"$@\"" -c

objects = main.o

plot.png: plot.gp out.times
	$(PLOT) $+

out.times: main Makefile
	> $@
	> log.main
	for nthreads in $$(seq 1 1 10); do \
		$(TIME) --format "$$nthreads %e %U" --append --output $@ ./$< --threads $$nthreads 1000000000 >>log.main ;\
	done

Out.txt: main
	./$< --test --threads 10 > $@

main: main.o

main.o: main.cpp

clean:
	$(RM) main Out.txt $(objects)
