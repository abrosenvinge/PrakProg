data = ARG1

set terminal svg size 800,800

set xlabel "Matrix size"
set ylabel "Time [s]"

set xrange [80:*]

plot data using 1:2 with lines title "Optimized", data using 1:3 with lines title "Unoptimized"
