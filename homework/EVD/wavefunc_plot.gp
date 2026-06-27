data = ARG1

set terminal svg size 800,800

set xlabel "r"
set ylabel "f(r)"

f1(x) = 2*x*exp(-x)
f2(x) = 1/sqrt(2) * x * (1 - 0.5*x) * exp(-0.5*x)
f3(x) = -2./(3.*sqrt(3.)) * (1. - 2./3. * x + 2./27. * x*x) * x * exp(-x/3.)
f4(x) = 0.25 * (1. - 3./4. * x + 1./8. * x*x - 1./192. * x**3) * x * exp(-x/4.)

set title "Hydrogen radial wavefunctions"
plot for [i=0:*] data index i title sprintf("f^{(%d)} (r)",i+1) with lines lw 3,\
	f1(x) with lines dashtype 2 lw 3 title "f^{(1)}_{exact}(r)",\
	f2(x) with lines dashtype 2 lw 3 title "f^{(2)}_{exact}(r)",\
	f3(x) with lines dashtype 2 lw 3 title "f^{(3)}_{exact}(r)",\
	f4(x) with lines dashtype 2 lw 3 title "f^{(4)}_{exact}(r)";
