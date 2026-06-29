set terminal svg background 'white' size 1300,800

data = ARG1

set xlabel "N"


set multiplot layout 2,2 columns

set title "Unit circle" 
plot data index 0 using 1:3 with lines lw 2 title "LCG",\
	 data index 0 using 1:6 with lines lw 2 title "std::uniform",\
	 data index 0 using 1:9 with lines lw 2 title "quasi",\
	 data index 0 using 1:2 with lines linetype 8 dashtype 2 lw 2 title "Actual value";

f(x) = n/sqrt(x)
fit f(x) data index 0 using 1:5 via n

set title "Error"

plot [*:*][-0.002:*] data index 0 using 1:5 with lines lw 2 title "LCG actual",\
	data index 0 using 1:8 with lines linetype 6 lw 2 title "std::uniform actual",\
	data index 0 using 1:11 with lines linetype 6 lw 2 title "quasi actual",\
	f(x) with lines linetype 8 lw 2 title "LCG fit",\
	data index 0 using 1:4 with lines dashtype 2 lw 2 title "LCG estimate",\
	data index 0 using 1:7 with lines dashtype 2 lw 2 title "std::uniform estimate",\
	data index 0 using 1:10 with lines dashtype 2 lw 2 title "quasi estimate";

set title "Disk potential" 
plot data index 1 using 1:3 with lines lw 2 title "LCG",\
	 data index 1 using 1:6 with lines lw 2 title "std::uniform",\
	 data index 1 using 1:9 with lines lw 2 title "quasi",\
	 data index 1 using 1:2 with lines linetype 8 dashtype 2 lw 2 title "Actual value";

f(x) = n/sqrt(x)
fit f(x) data index 1 using 1:5 via n

set title "Error"

plot [*:*][-0.002:*] data index 1 using 1:5 with lines lw 2 title "LCG actual",\
	data index 1 using 1:8 with lines linetype 6 lw 2 title "std::uniform actual",\
	data index 1 using 1:11 with lines linetype 6 lw 2 title "quasi actual",\
	f(x) with lines linetype 8 lw 2 title "LCG fit",\
	data index 1 using 1:4 with lines dashtype 2 lw 2 title "LCG estimate",\
	data index 1 using 1:7 with lines dashtype 2 lw 2 title "std::uniform estimate",\
	data index 1 using 1:10 with lines dashtype 2 lw 2 title "quasi estimate";
