# Linear equations
Project can be built using
```sh
make
```
and cleaned using
```sh
make clean
```

Making [scaling.data](./scaling.data) can take a bit with the current numbers.

## Requirements
- ```gnuplot``` 

## a
The Monte-Carlo integration algorithm is implemented in [monte_carlo.hpp](./monte_carlo.hpp) and [monte_carlo.cpp](./monte_carlo.cpp) in the ```plainmc``` function. The linear congruential generator is implemented in the ```LCG```. [Out.txt](./Out.txt) shows the calculated values of some integrals. The values for this part are listed under LCG.

Unit circle refers to the area of a unit circle. It is found by integrating the indicator function f(x,y) = ((x*x + y*y) <= 1) from -1 to 1 along both x and y. 

Disk potential refers to the integral of the function which is 

```math
    f(x,y) = \frac{1}{\sqrt{x^2 + y^2 +1}}
```
for $x^2 + y^2 <= 1$ and $0$ otherwise, which is rougly the electrical potential at a point directly above a uniformly charged disk.

Ellipsoid is volume of an ellipsoid and is found integrating an indicator function like for the unit circle.

Difficult integral is the integral from part b in the homework.

Gaussian is the function
```math
f(x,y) = \exp(-(x-1)^2 + (y+1)^2).
```

The error of the first two integrals as a function of the number of points is plotted in [scaling_plot.svg](./scaling_plot.svg). A fit to the relative error shows that it scales as $1/\sqrt(N)$ as expected. Also, the error estimate seems to be close to the actual error.

## b
The quasi-random integration is implemented in ```quasirandmc```. The results of the above integrals using the quasi random integrator can be seen in [Out.txt](./Out.txt) under quasi. Additionally, the result using the standard library random number generator ```std::uniform_distribution``` can be seen under uniform. Both of these are also plotted in [scaling_plot.svg](./scaling_plot.svg). Notably, the quasi random integrator does significantly better than both lcg and uniform integrator (which are about equal).

## c
Stratified sampling is implemented in the ```stratified``` and ```stratified_recursive``` functions. The results can be seen in [Out.txt](./Out.txt) under stratified. It does not seem better than the other methods.
