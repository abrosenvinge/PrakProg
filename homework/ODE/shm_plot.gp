set terminal svg size 1200,800

data = ARG1

set multiplot layout 2,1

set xlabel "x"
set ylabel "y"
set title "u" 

plot [-0.1:2.1] data using 1:2 with points title "RK45",\
	cos(x) with lines title "cos"
	
set title "u'" 

plot [-0.1:2.1] data using 1:3 with points title "RK45",\
	-sin(x) with lines title "-sin"
