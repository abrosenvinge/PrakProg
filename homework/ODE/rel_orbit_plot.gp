set terminal png size 1440,1440

data = ARG1

set xlabel "x"
set ylabel "y"
set title "Relativistic orbit" 

plot [-2:2][-2:2] data index 0 using (1/$2)*cos($1):(1/$2)*sin($1) with lines title "Newtonian",\
		data index 1 using (1/$2)*cos($1):(1/$2)*sin($1) with lines title "Eliptical",\
		data index 2 using (1/$2)*cos($1):(1/$2)*sin($1) with lines title "Relativistic precession"
