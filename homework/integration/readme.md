# Integration
Project can be built using
```sh
make
```
and cleaned using
```sh
make clean
```
## Requirements
- ```gnuplot``` 
- ```python``` with ```scipy``` and ```numpy``` installed

## a
This integration algorithm is implemented in [integration.hpp](./integration.hpp) and [integration.cpp](./integration.cpp) in the function ```integration``` which calls the recursive adaptive integration routine ```rai```. [Out.txt](./Out.txt) shows the results of some integrals (under Ordinary for this part). (Note that the third integral in the homework should be pi/4, not pi/2 as the homework says.) Output is made using the [print_script.sh](./print_script.sh) script because python needs to be called later.

The error function is implemented in the ```erf``` function. A plot can be seen in [erf_plot.svg](./erf_plot.svg) along with the error function from the standard library which matches well.

The error in $erf(1)$ can be seen in [erf_error_plot.svg](./erf_error_plot.svg), which shows that the error decreases with acc.

## b
The Clenshaw-Curtis quadrature is implemented in the function ```Clenshaw_Curtis_quad``` which reuses the ```rai``` routine from above. Infinite limits are implemented using the function ```transform_inf_interval``` which does an appropriate transformation to f if either limit is infinite, and is called at the start of each integration routine.

[Out.txt](./Out.txt) shows the results of the Clenshaw-Curtis quadrature under CC and the results of ```scipy```'s ```quad``` under Scipy. CC and ordinary use a similar number of evaluations except for the two integrals of function that diverge. Here the ordinary method uses significantly more. Scipy uses slightly more evaluations in general, but the result has much smaller error.

## c
The estimated error is simply returned from the ```rai``` routine. It can also be seen in [Out.txt](./Out.txt). In general it seems to be a reasonable estimate of the actual error.
