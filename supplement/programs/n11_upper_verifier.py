from itertools import combinations, permutations
from pathlib import Path

n=11
pairs=[(i,j) for i in range(n) for j in range(i+1,n)]
eid={e:k for k,e in enumerate(pairs)}
q3=[(a,b) for a in range(8) for bit in (1,2,4) if (b:=a^bit)>a]
q3=[tuple(sorted(e)) for e in q3]
local=[(i,j) for i in range(8) for j in range(i+1,8)]
base=set()
for p in permutations(range(8)):
    bm=0
    for a,b in q3:
        x,y=sorted((p[a],p[b])); bm |= 1 << eid[(x,y)]
    base.add(bm)
assert len(base)==840
cubes=set()
for S in combinations(range(n),8):
    for bm in base:
        cm=0
        for a,b in local:
            if bm & (1 << eid[(a,b)]):
                x,y=S[a],S[b]; cm |= 1<<eid[tuple(sorted((x,y)))]
        cubes.add(cm)
assert len(cubes)==138600
initial_edges=[(a,b) for a,b in q3 if True]
initial=[(0,2),(0,4),(1,3),(1,5),(2,3),(2,6),(3,7),(4,5),(4,6),(5,7),(6,7),
         (3,5),(3,8),(3,10),(4,8),(6,8),(7,10),(8,9),(9,10)]
add=[(0,1),(0,8),(2,5),(0,7),(0,9),(1,6),(1,8),(0,3),(0,5),(0,6),(0,10),(1,2),(1,4),(1,7),(1,9),(1,10),(2,4),(2,7),(2,8),(2,9),(2,10),(3,4),(3,6),(3,9),(4,7),(4,9),(4,10),(5,6),(5,8),(5,9),(5,10),(6,9),(6,10),(7,8),(7,9),(8,10)]
g=0
for e in initial:
    g |= 1<<eid[tuple(sorted(e))]
qfree=sum((c & ~g)==0 for c in cubes)
print('cubes',len(cubes),'initial_edges',g.bit_count(),'initial_Q3',qfree)
assert len(initial)==19 and g.bit_count()==19, g.bit_count()
assert qfree==0
for k,e in enumerate(add,1):
    ee=1<<eid[tuple(sorted(e))]
    assert not g & ee, ('repeated',k,e)
    witnesses=[c for c in cubes if (c&ee) and (c&~g)==ee]
    assert witnesses, ('no witness',k,e)
    g |= ee
assert len(add)==36 and g.bit_count()==55
assert g == (1<<55)-1
print('PASS n11 19-edge certificate')
