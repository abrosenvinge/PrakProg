set terminal svg background 'white' size 800,1000

data = ARG1

set multiplot layout 3,1 title ARG2

set xlabel "x"
set ylabel "f(x)"

set title "Function"
plot data index 0 using 1:2 with points title "Known points",\
	data index 1 using 1:2 with lines lw 2 title "Linear interpolation",\
	data index 1 using 1:5 with lines lw 2 title "Quadratic interpolation",\
	data index 1 using 1:8 with lines lw 2 title "Functional style quadratic"

set title "Anti-derivative"
set ylabel "F(x)"
plot data index 0 using 1:3 with points title "Known points",\
	data index 1 using 1:3 with lines lw 2 title "Linear interpolation",\
	data index 1 using 1:6 with lines lw 2 title "Quadratic interpolation"

set title "Derivative"
set ylabel "f'(x)"
plot data index 0 using 1:4 with points title "Known points",\
	data index 1 using 1:4 with lines lw 2 title "Linear interpolation",\
	data index 1 using 1:7 with lines lw 2 title "Quadratic interpolation"
