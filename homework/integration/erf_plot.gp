set terminal svg background 'white'

data = ARG1

set xlabel "z"
set ylabel "erf(z)"
set title "erf" 

plot data using 1:2 with lines lw 4 lt 3 title "calculated",\
	 data using 1:3 with lines dashtype 2 lt 8 lw 4 title "actual";
