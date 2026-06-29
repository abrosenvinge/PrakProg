set terminal svg background 'white' size 500,500

data = ARG1

set xlabel "N"
set ylabel "Time"

f(x) = a * x**3
fit f(x) data using 1:2 via a

plot data using 1:2 with points title "Measured", f(x) with line title "Fit"
