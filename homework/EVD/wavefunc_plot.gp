data = ARG1

set terminal png size 1920,1080

set xlabel "r"
set ylabel "f(r)"

set title "Hydrogen radial wavefunctions"
plot for [i=0:*] data index i title sprintf("f^{(%d)} (r)",i) with lines
