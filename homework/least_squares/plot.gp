set terminal svg background 'white' size 800,600
load ARG1 # loads the values of a and lambda from params.txt

f(t) = a * exp(-lambda * t)
fplus(t) = (a + da) * exp(-(lambda + dlambda) * t)
fminus(t) = (a - da) * exp(-(lambda - dlambda) * t)

set xlabel "t [days]"
set ylabel "Activity of ThX [rel. units]"

plot f(x) lw 2 title "F_c(t)",\
	fplus(x) lw 2 title "F_{c+dc}(t)",\
	fminus(x) lw 2 title "F_{c-dc}(t)",\
	"ThX_decay.data" using 1:2:3 with errorbars title "Data";

