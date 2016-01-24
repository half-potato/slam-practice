from fsensor import inverse, noisyFn, linear, poly
import numpy as np
from numpy import array
import matplotlib.pyplot as plt
import random

#Rover going across the desert ea
dt = 1
a = 1.5

model = linear(a, 0.0)

#Odometry data
"""oData = noisyFn(linear(0, 1.0), 0.05, xrange(0, 100, dt))
#GPS data
sData = noisyFn(model, 40, xrange(0, 100, dt))
#Not how you get covariance apparently?
Ex = array([
        [np.var(oData, ddof=1) , np.std(oData, ddof=1) * np.std(sData, ddof=1)],
        [np.std(sData, ddof=1) * np.std(oData, ddof=1) , np.var(sData, ddof=1)]
])
Ez = Ex[1][1]
"""
Ez = 10**2
Ex = 0.05 * array([[(dt**4)/4, (dt**3)/2], [(dt**3)/2, dt**2]])
A = array([[1.0, dt], [0.0, 1.0]])
B = array([[dt**2.0 / 2.0, dt]]).T
C = array([[1, 0]])
oData = [array([[0.0, 0.0]]).T]
sData = []
oNoiseMag = 0.05
sNoiseMag = 1000
for i in xrange(0, 100, dt):
    onoise = oNoiseMag * array([[(dt**2) /2 * (random.random()-.5), dt*(random.random())]]).T
    oData.append(A.dot(oData[-1]) + B.dot(a) + onoise)
    sData.append(C.dot(oData[-1] + sNoiseMag * (random.random()-.5)))
oData = oData[1:]
#state
xhat = [array([[0.0, 0.0]]).T]
#prediction error
E = [Ex]
#gain
g = [array([[0.0, 0.0]]).T]
r = np.var(sData, ddof=1)

for i in xrange(0, 100, dt):
    k = i+1
    xhat.append(A.dot(xhat[k-1]) + B.dot(a))#oData[i]))
    E.append((A.dot(E[k-1])).dot(A.T) + Ex)
    g.append((E[k].dot(C.T)).dot(np.linalg.inv(C.dot(E[k]).dot(C.T) + Ez)))
    xhat[k] = xhat[k] + g[k].dot((sData[i] - C.dot(xhat[k])))
    E[k] = (np.eye(2) - g[k].dot(C)).dot(E[k])

xhat = xhat[1:]
z = []
for i in range(len(xhat)):
    z.append(xhat[i][0])

X = xrange(0, 100, dt)
kman = plt.plot(X, z)
for i in range(len(sData)):
    sData[i] = sData[i][0]
gps = plt.plot(X, sData)
for i in range(len(oData)):
    oData[i] = oData[i][0]
act = plt.plot(X, oData)
plt.setp(kman, color = "g", lw=2.0)
plt.setp(gps, color = "r")
plt.setp(act, color = "b")
plt.show()
