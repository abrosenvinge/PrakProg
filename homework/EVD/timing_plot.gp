data = ARG1

set terminal png size 1920,1080

set xlabel "Matrix size"
set ylabel "Time [s]"

set xrange [80:*]

plot data using 1:2 with lines title "Optimized", data using 1:3 with lines title "Unoptimized"
