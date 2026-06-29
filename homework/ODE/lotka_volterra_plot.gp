set terminal svg background 'white' size 800,600

data = ARG1

set xlabel "t"
set ylabel ""
set title "Lotka-Volterra" 

plot [-0.1:15.1] data using 1:2 with lines lw 2 title "x",\
	data using 1:3 with lines lw 2 title "y";
