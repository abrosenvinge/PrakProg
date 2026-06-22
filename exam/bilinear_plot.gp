set terminal svg size 1000,1000

data = ARG1

#set view 65,20

set xlabel "x"
set ylabel "y"
set zlabel "z"

set xyplane 0

splot data index 1 using 1:2:3 with points pointtype 2 pointsize 1 title "Interpolation",\
	data index 0 using 1:2:3 with points pointtype 7 pointsize 1 title "Input";
