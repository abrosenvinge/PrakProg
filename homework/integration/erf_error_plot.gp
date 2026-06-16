set terminal svg

data = ARG1

set xlabel "acc"
set ylabel "|std::erf(1.) - erf(1., acc)|"
set title "erf(1) error" 

set logscale x
set logscale y
set xrange reverse

plot data using 1:2 with lines lw 4 title "",\
