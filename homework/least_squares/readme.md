# Ordinary least-squares
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

## Solution
The least-squares fit algorithm is implemented in the function ```lsfit``` in [least_squares.hpp](./least_squares.hpp). The fit parameters can be seen in [Out.txt](./Out.txt) along with the uncertainties and the covariance matrix. The estimated half-life does not agree with the modern value within the determined uncertainty.

[plot.svg](./plot.svg) shows a plot of the fit and the data along with the fit where the parameters have been shifted by an uncertainty. The fit is very reasonable.
