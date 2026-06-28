# ODE
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
A Runge-Kutta 45 stepper is implemented in the function ```rkstep45``` in [ode.hpp](./ode.hpp) and [ode.cpp](./ode.cpp). A dynamic stepsize driver is implemented in ```driver``` which returns the x and y values of each step. ```endpoint_driver``` is exactly the same but returns only y at the last step. It is used for the shooting method in the [root finding homework](../root_finding).

[shm_plot.svg](./shm_plot.svg) shows the solution to a simple harmonic oscillator $u'' = -u$.

[lotka_volterra_plot.svg](./lotka_volterra_plot.svg) shows the solution of the Lotka-Volterra system from the [solve_ivp manual](https://docs.scipy.org/doc/scipy/reference/generated/scipy.integrate.solve_ivp.html). The solution seems to match.

## b
[rel_orbit_plot.svg](./rel_orbit_plot.svg) shows the solution of the relativistic orbit. The solutions are as expected, with precession around the origin when $\epsilon = 0.01$. For this part a ```max_step``` parameter had to be implemented, such that enough steps were generated for plotting purposes.

## c
This can be seen in [three_body_plot.svg](./three_body_plot.svg).
