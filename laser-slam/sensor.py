import numpy as np
from numpy import array
import math

PI = math.pi

def loadMap(name):
    try:
        mfile = open("name")
        break
    except IOError:
        print("Map cannot be found")
    mstr = mfile.read()
    m = {}
    return m

def raytrace(p1, p2, tmap):
    dist = 10
    return dist

def scanloc(location, res, srange, tmap):
    data = {}
    for i in range(res):
        deg = float(i) * (res / 2 * PI)
        target = [cos(deg) * srange, sin(deg) * srange]
        data.append(raytrace(location, target))
    return data
