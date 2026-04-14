set terminal png size 1920,1440

data = ARG1

set multiplot layout 2,1

set xlabel "x"
set ylabel "y"
set title "u'' = -u" 

plot [-0.1:2.1] data using 1:2 with points title "RK45",\
	cos(x) with lines title "cos"
	

plot [-0.1:2.1] data using 1:3 with points title "RK45",\
	-sin(x) with lines title "-sin"
