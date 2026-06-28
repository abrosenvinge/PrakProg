# Minimization
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
The minimization algorithm is implemented in [minimization.hpp](./minimization.hpp) and [minimization.cpp](./minimization.cpp) in the function ```min_newton_fd```. The gradient and Hessian are computed by finite-differences by ```gradient_fd``` and ```hessian_fd``` respectively.

Optimization of various functions can be seen in [Out.txt](./Out.txt) along with the number iterations and function evaluations. The algorithm from this part is under the column "Forward diff".

## b
Least squares fitting is implemented in ```leastsq_fit```. The fit to the higgs data can be seen in [higgs_plot.svg](./higgs_plot.svg), which fits well. The optimal parameters can be seen in [Out.txt](./Out.txt). The mass is 126 GeV which is reasonably close to the accepted value of 125.3±0.6 GeV. The fit is quite sensitive to the initial parameters.

## c
Unoptimized versions of the central difference gradient and hessian are implemented in ```gradient_cd``` and ```hessian_cd```. An optimized version that computes the hessian and gradient simultaneously is implemented in ```grad_hess_cd```. (Note that I think there may be an error in the book. At least the gradient and hessian described in the section do not share function calls if $\delta x_i$ is the same in the two equations ((14) and (15)). Therefore, I take the gradient step size to be twice as large.)

The result of these two methods can be seen in [Out.txt](./Out.txt). It is seen that they both find the correct minima in all cases, but use more function evaluations as expected. The optimized version uses significantly fewer function calls. This is both due the reuse of values in the computation of the gradient, but also due to computing only half the Hessian since it is symmetric.
