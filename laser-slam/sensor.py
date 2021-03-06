import numpy as np
from numpy import array
import math
import re

PI = math.pi

def loadMap(name):
    try:
        f = open("map")
    except IOError:
        print("Map could not be found")
        return -1
    s = f.read()
    m = s.split("\n")
    markers = []
    rlocs = []
    for i in m:
        markers.append([m.start() for m in re.finditer("l", i)])
        locs = [m.start() for m in re.finditer(r"([0-9])", i)]
        rlocs.append({p: int(i[p]) for p in locs})
    return (markers, rlocs, s)
    #markers is an array of indicies for the landmarks
    #rlocs is an array of [(indice of number), (number)]

def getRobotPos(index, rlocs):
    for i in range(len(rlocs)):
        if index in rlocs[i]:
            return [rlocs[i][index], i]
    return -1

#res is an int, p1 and p2 are in the same dimensions as the marker (meaning markers[p1[0]][p1[1]] refers to the tile it is looking for
def raytrace(p1, p2, markers, res):
    #Returns hit closest to p1
    diff = np.subtract(p2, p1)
    diffx = diff[0]
    if diffx != 0 and diff[1] != 0:
        slope = diff[1] / diffx
        #directions
        drs = slope / abs(slope)
        drx = diffx / abs(diffx)
        for i in range(int(math.ceil(abs(diffx)) * res)):
            idiffx = (diffx / math.ceil(abs(diffx))) * float(i / res)
            y = slope * idiffx + p1[1]
            x = idiffx + p1[0]

            ix = x
            iy = y
            if drx == -1:
                ix = int(math.ceil(ix))
            else:
                ix = int(math.floor(ix))

            if drs == -1:
                iy = int(math.floor(iy))
            else:
                iy = int(math.ceil(iy))
            if iy in markers[ix]:
                return math.sqrt(x**2 + y**2)
    return -1


def scanloc(location, startA, finishA, res, srange, markers, tisize):
    data = []
    for i in range(res):
        deg = startA + float(i) * (res/(finishA-startA))
        target = [math.cos(deg) * srange, math.sin(deg) * srange]
        data.append(raytrace(location, target, markers, tisize))
    return data
#usage
(markers, rlocs, m) = loadMap("map")
p1 = getRobotPos(9, rlocs)
print(scanloc(p1, 0., 2.*PI, 100, 10., markers, 1.))
