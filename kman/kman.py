from fsensor import inverse, noisyFn, linear, poly
import numpy as np
from numpy import array
import matplotlib.pyplot as plt
import random

#Rover going across the desert ea
dt = 1
a = 1.5
X = range(0, 10, dt)

#COVARIANCE ?
oNoiseMag = 1.5
sNoiseMag = 1000
Ez = sNoiseMag**2
#Since dt = 1, this array = [1/4, 1/2;
#                           [1/2,   1]
Ex = oNoiseMag * array([[(dt**4)/4, (dt**3)/2], [(dt**3)/2, dt**2]])

#INTERACTION MODEL
# State - State model
A = array([[1.0, dt], [0.0, 1.0]])
# Acceleration/Control model
B = array([[dt**2.0 / 2.0, dt]]).T
# What the sensor sees of the state - model
C = array([[1, 0]])

# GENERATE DATA
oData = [array([[0.0, 0.0]]).T]
sData = []
for i in X:
    # Process noise
    onoise = oNoiseMag * array([[(dt**2) /2 * (random.random()-.5), dt*(random.random())]]).T
    # Generate next position. POS + DPOS + PROCESS_NOISE
    oData.append(A.dot(oData[-1]) + B.dot(a) + onoise)
    # oData[-1] = ^that line. The sdata is the X component of the robot's state + process noise
    sData.append(C.dot(oData[-1] + sNoiseMag * (random.random()-.5)))
#Remove the starting position
oData = oData[1:]

# FROM HERE ON OUT, EVERYTHING IS PREDICTED FROM LASER RANGE FINDER DATA
#State prediction
xhat = [array([[0.0, 0.0]]).T]
#Predicted error
E = [Ex]
#Kalman gain
g = [array([[0.0, 0.0]]).T]

# RUN KALMAN FILTER
for i in X:
    #Cause in math, an array starts at 1
    k = i+1
    xhat.append(A.dot(xhat[k-1]) + B.dot(a))
    print("Model xhat:", xhat[k])
    # Error builds up based on how much the robot moves because it accumulates error. Previous error + accumulated error according to model + error from instruments = predicted error
    E.append((A.dot(E[k-1])).dot(A.T) + Ex)
    print("Model prediction error:", E[k])
    print(E[k], C.T, E[k].dot(C.T))
    #g.append(array([[(E[k][0][0]+E[k][1][0])/(E[k][0][0]+Ez)]]))
    g.append((E[k].dot(C.T)).dot(np.linalg.inv(C.dot(E[k]).dot(C.T) + Ez)))
    print("Kalman gain:", g[k])
    #Correct the model with sensor data
    #xhat + difference between model and sensor data multiplied by confidence in data
    xhat[k] = xhat[k] + g[k].dot((sData[i] - C.dot(xhat[k])))
    print("xhat with those gains:", xhat[k])
    #subtract the kalman gain multiplied by the covariance from one
    E[k] = (np.eye(2) - g[k].dot(C)).dot(E[k])
    print("Corrected prediction error:", E[k])
#Remove starting prediction
xhat = xhat[1:]

# PLOT
#New array z of only the predicted x positions
z = []
for i in xhat:
    z.append(i[0])
kman = plt.plot(X, z)

#New array zs of the laser range finder data with only x positions
zs = []
for i in sData:
    zs.append(i[0])
gps = plt.plot(X, zs)

#New array zo of the actual state data with only x positions
zo = []
for i in oData:
    zo.append(i[0])
act = plt.plot(X, zo)
#Color code
plt.setp(kman, color = "g", lw=2.0)
plt.setp(gps, color = "r")
plt.setp(act, color = "b")

plt.show()
