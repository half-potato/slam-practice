
# coding: utf-8

# In[21]:

import re
import numpy as np


# In[30]:

try:
    f = open("map")
except IOError:
    print("Map could not be found")
s = f.read()
m = s.split("\n")
markers = []
rlocs = []
for i in m:
    markers.append([m.start() for m in re.finditer("l", i)])
    locs = [m.start() for m in re.finditer(r"([0-9])", i)]
    rlocs.append([[p, i[p]] for p in locs])
#markers is an array of indicies for the landmarks
#rlocs is an array of [(indice of number), (number)]


# In[ ]:

def raytrace(p1, p2):
    slope = np.subtract(p2, p1)


# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:



