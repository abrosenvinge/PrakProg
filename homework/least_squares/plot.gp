set terminal png size 1920,1080
load ARG1 # loads the values of a and lambda from Out.txt

f(t) = a * exp(-lambda * t)
fplus(t) = (a + da) * exp(-(lambda + dlambda) * t)
fminus(t) = (a - da) * exp(-(lambda - dlambda) * t)

set xlabel "t [days]"
set ylabel "Activity of ThX [rel. units]"

plot "ThX_decay.data" using 1:2:3 with errorbars title "Data", \
	f(x) title "F_c(t)",\
	fplus(x) title "F_{c+dc}(t)",\
	fminus(x) title "F_{c-dc}(t)";
