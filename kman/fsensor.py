import numpy as np
import random as rm
import operator
#import matplotlib.pyplot as plt
#generates fake sensor output

def linear(a, b):
    return lambda x: a*x+b

#This array: [1, 2, 4] represents x^2 + 2x + 4
def poly(ar):
    print(ar)
    polyar = [[x, len(ar) - i - 1] for i, x in enumerate(ar)]
    return lambda v: sum([i[0] * v ** i[1] for i in polyar])

def inverse(a, b, c):
    return lambda x: a / x**b + c

def noisyFn(fn, noiseAmp, vrange):
    return [fn(x) + (noiseAmp * rm.random()) - noiseAmp / 2 for x in vrange]

#plt.plot(xrange(1, 100, 1), noisyFn(inverse(100, 1, 1), 3, xrange(1, 100, 1)))
#plt.plot(xrange(1, 100, 1), map(inverse(100, 1, 1), xrange(1, 100, 1)))
#plt.plot(xrange(-100, 100, 1), [poly([1, 0, 0, 0])(x) for x in xrange(-100, 100, 1)])

#plt.show()
