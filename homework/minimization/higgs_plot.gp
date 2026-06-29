set terminal svg background 'white' size 500,500

fit_data = ARG1
cern_data = ARG2

set xlabel "E"
set ylabel "\sigma"

plot cern_data using 1:2:3 with yerrorbars title "Experimental data",\
	fit_data using 1:2 with lines title "Fit";
