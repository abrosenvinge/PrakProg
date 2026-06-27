data = ARG1

set terminal svg size 800,800

set xlabel "dr"
set ylabel "E_0"

set xrange reverse

set title "E_0 as function of dr"
plot data using 1:3 with points title "E_0"
