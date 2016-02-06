import sensor
import numpy as np
import numpy.linalg as la
import math

def move(startLoc, endLoc, rlocs, markers, robotSpeed, processNoise, odoNoise, lasNoise):
    actual = []
    odo = []
    las = []
    start = np.multiply(getRobotPos(startLoc, rlocs), 10.)
    end = np.multiply(getRobotPos(endLoc, rlocs), 10.)
    diff = np.subtract(end, start)
    slope = la.norm(diff)
    dist = math.sqrt(np.dot(diff, diff))
    hasArrived = False
    while !hasArrived:
        movement = np.multply(dist, slope)
        hasArrived = True
    #This code does not use the linear algebra described.

def simulate
