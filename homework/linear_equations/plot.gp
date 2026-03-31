set terminal png size 1920,1080
set output OUTFILE

set xlabel "N"
set ylabel "Time"

f(x) = a * x**3
fit f(x) INFILE using 1:2 via a

plot INFILE using 1:2 with points title "Measured", f(x) with line title "Fit"
