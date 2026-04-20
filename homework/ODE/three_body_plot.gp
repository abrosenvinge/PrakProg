set terminal png size 1440,1440

data = ARG1

set xlabel "x"
set ylabel "y"
set title "Figure eight" 

plot data using 7:8 with points title "Body 1",\
	data using 9:10 with points title "Body 2",\
	data using 11:12 with points title "Body 3"
