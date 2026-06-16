set terminal svg

data = ARG1

set xlabel "z"
set ylabel "erf(z)"
set title "erf" 

plot data using 1:2 with lines lw 4 title "calculated",\
	 data using 1:3 with lines dashtype 2 lw 4 title "actual";
