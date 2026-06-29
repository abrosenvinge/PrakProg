set terminal svg size 900,900

data = ARG1

set view 55,65

stats data nonuniform matrix nooutput prefix "A";

set xlabel "x"
set ylabel "y"
set zlabel "z"

set xyplane 0


set multiplot layout 2,2

splot data index 2 nonuniform matrix using 1:2:3 with surface lw 0.5 lc rgb "grey" title "Correct function",\
	data index 0 nonuniform matrix using 1:2:(A_min) with surface title "Grid",\
	data index 0 nonuniform matrix using 1:2:3 with surface lw 1 lc rgb "red" title "Samples";

splot data index 1 nonuniform matrix using 1:2:3 with surface lw 0.5 lc rgb "orange" title "Bi-linear interpolation";
	#data index 0 nonuniform matrix using 1:2:3 with surface lw 1 lc rgb "red" title "Samples";

splot data index 4 nonuniform matrix using 1:2:3 with surface lw 0.5 title "Exact integral";

splot data index 3 nonuniform matrix using 1:2:3 with surface lw 0.5 lc rgb "blue" title "Interpolated integral";


