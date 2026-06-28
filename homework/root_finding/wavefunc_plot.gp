set terminal svg size 800,800

data = ARG1

set xlabel "r"
set ylabel "Wavefunc"

plot for [i=1:*] data index i using 1:2 with lines lw 4 title columnhead,\
	 data index 0 using 1:2 with lines dashtype 2 lt 8 lw 4 title "Exact wavefunction";
