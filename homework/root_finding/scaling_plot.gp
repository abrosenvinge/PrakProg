set terminal svg size 1300,800

rmaxdata = ARG1
rmindata = ARG2
accdata = ARG3
epsdata = ARG4

set ylabel "E_0"

set multiplot layout 2,2 columns

set xlabel "r_{max}"
plot [*:*][-0.52:*] rmaxdata using 1:2 with lines lw 2 title "";

set xlabel "r_{min}"
set xrange reverse
plot rmindata using 1:2 with lines lw 2 title "";

set xlabel "acc"
set xrange reverse
plot accdata using 1:2 with lines lw 2 title "";

set xlabel "eps"
set xrange reverse
plot epsdata using 1:2 with lines lw 2 title "";
