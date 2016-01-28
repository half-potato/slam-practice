import numpy as np
from numpy import array
import math

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
    return (markers, rlocs, m)
    #markers is an array of indicies for the landmarks
    #rlocs is an array of [(indice of number), (number)]

def getRobotPos(index, rlocs):
    for i in range(len(rlocs)):
        if index in rlocs[i]:
            return [rlocs[i][index], i]
    return -1

def raytrace(p1, p2, markers, tisize):
    #Returns hit closest to p1
    diff = np.subtract(p2, p1)
    diffx = diff[0]
    print(diff)
    if diffx != 0 and slope[1] != 0:
        slope = diff[1] / diffx
        for i in range(int(math.ceil(abs(diffx)))):
            idiffx = (diffx / math.ceil(abs(diffx))) * float(i)
            y = slope * idiffx + p1[1]
            x = idiffx + p1[0]
            mx, my = math.ceil(x), math.ceil(y)
            lx, ly = math.floor(x), math.floor(y)

            dirx, dirslope = int(diffx / abs(diffx)), int(slope / abs(slope))

            cases = {}

            tilesAround = [[lx, ly], [mx, ly], [mx, my], [lx, my]]

            #get the start index of colliding cubes
            us = 2 * (slope / abs(slope)) + (diffx / abs(diffx))
            #map it to the indicies
            cases = {3:0.0, -3:1.0, 1:2.0, -1:3.0}
            s = cases[us]

            # check around the line and calculate what order to go through the tiles around the line
            for xy in range(4):
                p = xy + s
                if p > 3:
                    p = p - 4
                    tindex = tilesAround[int(p)]
                    row = markers[tindex[0]]
                    if tindex[1] in row:
                        return math.sqrt(tindex[0]**2 + tindex[1]**2)
        return -1

print(s)
p1 = getRobotPos(9, rlocs)
p2 = [11.0, 0.0]
for i in range(20):
    print("ray", raytrace(p1, [float(i + 0.01), 0.0], markers, 1.))

def scanloc(location, res, srange, tmap):
    data = {}
    for i in range(res):
        deg = float(i) * (res / 2 * PI)
        target = [cos(deg) * srange, sin(deg) * srange]
        data.append(raytrace(location, target))
        return data
