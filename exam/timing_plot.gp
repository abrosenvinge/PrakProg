set terminal svg size 500,500

data = ARG1

set xlabel "N"
set ylabel "Time [s]"

set xyplane 0

plot data using 1:2 with lines lw 3 title "Normal",\
	data using 1:3 with lines lw 3 title "Grid mode";
