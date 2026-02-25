table_data = ARG1
generated_data = ARG2

print generated_data
print table_data

set terminal png size 1920,1080
set output out

set xlabel "x"
set ylabel "y"
set autoscale noextend
set offsets graph 0.05, graph 0.05, graph 0, graph 0 #left,right,top,bottom
set key top left
plot [][-5<*:*<5] table_data using 1:2 with points title "table", generated_data using 1:2 with line title "generated"
