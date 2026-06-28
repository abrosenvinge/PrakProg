import numpy as np
import scipy.integrate as si
import sys

args = sys.argv
fstr = args[1]
a = np.float64(args[2])
b = np.float64(args[3])

if fstr == '1/sqrt(x)': f = lambda x: 1/np.sqrt(x)
elif fstr == 'sqrt(x)': f = lambda x: np.sqrt(x)
elif fstr == 'sqrt(1-x*x)': f = lambda x: np.sqrt(1 - x*x)
elif fstr == 'ln(x)/sqrt(x)': f = lambda x: np.log(x)/np.sqrt(x)
elif fstr == 'exp(-x*x)': f = lambda x: np.exp(-x*x)
elif fstr == 'exp(-x)': f = lambda x: np.exp(-x)
else: exit()

res, err, info = si.quad(f, a, b, full_output = True)

print("Scipy", res, info['neval'], err)
