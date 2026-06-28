# Linear equations
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

## a
Linear spline interpolation is implemented in [splines.hpp](./splines.hpp) in the function ```lin_interp``` and the class ```LinearSpline```. It uses the binary search implemented in the function ```bin_search```. Plots of linear interpolations of some functions can be seen in [exp_plot.svg](./exp_plot.svg), [quadratic_plot.svg](./quadratic_plot.svg), and [sin_plot.svg](./sin_plot.svg). These plots also show derivative and anti-derivative implemented in the ```integral``` and ```deriv``` methods.

## b
Quadratic spline is implemented in the class ```QuadraticSpline```. The plots from a also show the quadratic spline and its (anti-)derivative. [Out.txt](./Out.txt) shows some tests of the QS compared to manually calculated coefficients.

## c
A functional version of the quadratic spline is implemented in ```quadratic_spline```.
