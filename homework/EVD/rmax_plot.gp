data = ARG1

set terminal png size 1920,1080

set xlabel "rmax"
set ylabel "E_0"

set yrange [-0.52:2.5]
set title "E_0 as function of rmax"
plot data using 2:3 with points title "E_0"
