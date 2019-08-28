# try to separate sequences using latent semantic analysis

import numpy as np

x = np.genfromtxt("result.txt", delimiter="\t", dtype=str)

# cut of counts and headers
# TODO: one should maybe use the counts as a significance/weight measure
x = x[1:, 1:]

# Cut off the lower-ranked PDs to make things cleaner.
# TODO: we might break PD in the middle but whatever
#x = x[:500, :]

# map labels into matrix dimensions
mods = {mod: idx for idx, mod in enumerate(sorted(set(x[:,0])))}
pds = {pd: idx for idx, pd in enumerate(sorted(set(x[:,1])))}

# ... and create matrix
a = np.zeros([len(mods), len(pds)])
for mod, pd in x:
   a[mods[mod], pds[pd]] = 1

# decompose the matrix
u,s,v = np.linalg.svd(a, full_matrices=True)

# reconstruct using 10 latent features
sum(a)
sum(np.round(u[:,:10].dot(np.diag(s).dot(v)[:10,:]))) 
# this has some mistakes but looks not too bad


# look at the latent features

# we need to somehow turn the real number back to discrete, so choose a random treshold
t_u = np.abs(u).mean() * 3
t_v = np.abs(v).mean() * 3
print t_u, t_v

for i in range(0, 10):
  print 'Sequence', i, 'weight: ', s[i]
  print 'Modules: ', [mod for mod,idx in mods.items() if abs(u[idx,i]) > t_u] 
  print 'Used on PDs: ', [pd for pd,idx in pds.items() if abs(v[i,idx]) > t_v] 







