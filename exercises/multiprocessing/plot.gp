DATA = ARG1
set terminal png size 1920,1080

set xlabel "Number of threads"
set ylabel "Time"

plot DATA using 1:2 with lines title notitle
