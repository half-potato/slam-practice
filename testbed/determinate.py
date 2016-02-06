import numpy as np
from numpy import array

def singleDet(arr):
    return arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0]

def x3Det(arr):
    return array(np.multiply(arr[0], [1, -1, 1])).dot([singleDet(arr[1:3, 1:3], 
