# ANN
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
The ann is implemented in [ann.hpp](./ann.hpp) and [ann.cpp](./ann.cpp). The virtual class ```ann``` describes a one hidden layer ann as described in the homework. The virtual methods ```activation```, ```activation_derivative```, ```activation_2nd_derivative```, and ```activation_anti_derivative``` describe the behaviour of the activation function and must be implemented by subclasses. Given these the gradient and hessian of the network response function wrt. the network parameters can be computed analytically. This is done in the ```gradient_p``` and ```hessian_p``` methods. Using this the gradient and hessian of the least square cost function can be computed exactly which is done is ```cost_grad_hess_C```.

The ```ann_Gaussian_wavelet``` subclass is a class to describe an ann with a Gaussian wavelet activation function. It implements the virtual methods ```activation```, ```activation_derivative```, ```activation_2nd_derivative```, and ```activation_anti_derivative``` for a Gaussian wavelet.

Using this the network can be trained using the minimization algorithm used in the [minimization homework](../minimization/), adapted to work with the ```cost_grad_hess_C``` computation of the gradient and Hessian. Training to using a least squares cost function is implemented in ```train_leastsq```. 

Training to approximate a linear function can be seen in [linear_plot.svg](./linear_plot.svg). Approximation of $g(x) = \cos(5x) \exp(-x^2)$ can be seen in [gauss_oscillation_plot.svg](./gauss_oscillation_plot.svg).

## b
Derivative, second derivative, and anti-derivative can all be determined exactly using the properties of the activation function. This is done in the ```deriv```, ```second_deriv```, ```anti_deriv``` methods. These can be seen for the above functions in the same plots.

## c
Training to approximate the solution to a differential equation is done in the ```train_diffeq``` method. It uses the integrator implemented in the [integration homework](../integration/) and the minimization algorithm implemented in the [minimization homework](../minimization/). An approximation of the solution of a harmonic oscillator $y'' = -y$ can be seen in [harm_oscillator_plot.svg](./harm_oscillator_plot.svg). 

Note that this method was found to be extremely sensitive to the initial parameters of the model. Therefore, I create the initial parameters using a cosine, which may be considered cheating.
