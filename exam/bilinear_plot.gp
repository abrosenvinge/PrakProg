set terminal svg size 1000,1000

data = ARG1

set view 55,45

stats data nonuniform matrix nooutput prefix "A";

set xlabel "x"
set ylabel "y"
set zlabel "z"

set xyplane 0

#set hidden3d

splot data index 0 nonuniform matrix using 1:2:(A_min) with lines title "Grid",\
	data index 1 nonuniform matrix using 1:2:3 with surface lc rgb "orange" title "Interpolation",\
	data index 0 nonuniform matrix using 1:2:3 with surface lc rgb "red" title "Input";
