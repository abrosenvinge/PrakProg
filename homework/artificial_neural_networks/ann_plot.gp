set terminal svg size 1000,1000

data = ARG1

set xlabel "x"
set ylabel ""

set multiplot layout 2,2

set title "Function"
plot data using 1:2 index 2 with lines lt 4 lw 4 title "ANN Response",\
	 data using 1:2 index 1 with lines lt 2 dashtype 3 lw 4 title "Initial parameters",\
	 data using 1:2 index 0 with lines lt 8 dashtype 2 lw 4 title "Training data";

set title "Anti derivative"
plot data using 1:3 index 2 with lines lt 4 lw 4 title "ANN Response",\
	 data using 1:3 index 1 with lines lt 2 dashtype 3 lw 4 title "Initial parameters",\
	 data using 1:3 index 0 with lines lt 8 dashtype 2 lw 4 title "Training data";

set title "Derivative"
plot data using 1:4 index 2 with lines lt 4 lw 4 title "ANN Response",\
	 data using 1:4 index 1 with lines lt 2 dashtype 3 lw 4 title "Initial parameters",\
	 data using 1:4 index 0 with lines lt 8 dashtype 2 lw 4 title "Training data";

set title "2nd derivative"
plot data using 1:5 index 2 with lines lt 4 lw 4 title "ANN Response",\
	 data using 1:5 index 1 with lines lt 2 dashtype 3 lw 4 title "Initial parameters",\
	 data using 1:5 index 0 with lines lt 8 dashtype 2 lw 4 title "Training data";
