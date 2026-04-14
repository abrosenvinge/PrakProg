set terminal png size 1920,1440

data = ARG1

set xlabel "x"
set ylabel "y"
set title "Lotka-Volterra" 

plot [-0.1:15.1] data using 1:2 with lines title "x",\
	data using 1:3 with lines title "y";
