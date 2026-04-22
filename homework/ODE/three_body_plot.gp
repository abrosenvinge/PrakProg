set terminal png size 1440,1440

data = ARG1

set xlabel "x"
set ylabel "y"
set title "Figure eight" 

set xrange [-1.5:1.5]
set yrange [-1.5:1.5]

plot data using 8:9 with points title "Body 1",\
	data using 10:11 with points title "Body 2",\
	data using 12:13 with points title "Body 3"
