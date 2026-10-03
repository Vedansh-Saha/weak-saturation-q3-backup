from itertools import combinations, permutations
from collections import Counter
import json

N=10
pairs=[(i,j) for i in range(N) for j in range(i+1,N)]
eid={e:k for k,e in enumerate(pairs)}
local=[(i,j) for i in range(8) for j in range(i+1,8)]
leid={e:k for k,e in enumerate(local)}
q3=[(a,a^d) for a in range(8) for d in (1,2,4) if a<(a^d)]
base=set()
for p in permutations(range(8)):
    m=0
    for a,b in q3:
        x,y=sorted((p[a],p[b])); m|=1<<leid[(x,y)]
    base.add(m)
core=set(q3)
cubes=set()
for S in combinations(range(N),8):
    for bm in base:
        em=set()
        for a,b in local:
            if bm>>leid[(a,b)]&1:
                x,y=sorted((S[a],S[b])); em.add((x,y))
        cubes.add(frozenset(em-core))
seed={(1,2),(1,9),(2,9),(3,5),(3,6),(6,8),(7,8)}
seq=[(0,1),(3,9),(0,7),(1,7),(0,3),(2,5),(4,7),(1,6),(2,7),(5,6),(2,4),(3,4),(0,5),(0,6),(1,4),(5,9),(4,9),(6,9),(0,9),(7,9),(2,8),(5,8),(1,8),(3,8),(4,8),(0,8),(8,9)]
active=set(seed)
rows=[]
for step,e0 in enumerate(seq,1):
    e=tuple(sorted(e0))
    if e==(0,1):
        rows.append((step,e,0,[], 'canonical completion of C'))
        active.add(e); continue
    ws=[m for m in cubes if e in m and (m-{e})<=active]
    min_size=min(map(len,ws));w=min(ws,key=lambda m:(len(m),sorted(m)))
    prereq=sorted(w-{e})
    rows.append((step,e,min_size-1,prereq,''))
    active.add(e)
print('seed decomposition: K3 on {1,2,9}, K3 on {6,7,8} after including core edge 67, plus 35 and 36.')
print('one-prerequisite activations:')
for r in rows:
    if r[2]==1: print(r)
print('rows:')
for r in rows: print(r)
phases=[('A',1,5),('B',6,12),('C',13,15),('D',16,20),('E',21,27)]
for name,a,b in phases:
    vals=[r[2] for r in rows if a<=r[0]<=b and r[0]>1]
    print('phase',name,'steps',a,'-',b,'prereq_sizes',vals)
