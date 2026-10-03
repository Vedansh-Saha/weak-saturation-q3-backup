from itertools import permutations
from collections import Counter
from math import comb

n=11
pairs=[(i,j) for i in range(n) for j in range(i+1,n)]
eid={e:k for k,e in enumerate(pairs)}
q=[(a,a^b) for a in range(8) for b in (1,2,4) if a<a^b]
core=set(tuple(sorted(e)) for e in q)
outs=[e for e in pairs if e not in core]

def aut_q3():
    aut=[]
    p=list(range(8))
    qset=set(core)
    for perm in permutations(range(8)):
        if all(tuple(sorted((perm[a],perm[b]))) in qset for a,b in q):
            aut.append(perm)
    return aut

auts=aut_q3()
stab=[]
for a in auts:
    if {a[0],a[1]}=={0,1}:
        stab.append(a)
assert len(auts)==48 and len(stab)==4
G=[]
for a in stab:
    for ov in permutations([8,9,10]):
        p=list(range(n))
        p[:8]=a
        p[8:]=ov
        mp=[]
        for e in outs:
            x,y=sorted((p[e[0]],p[e[1]]))
            mp.append(outs.index((x,y)))
        G.append(tuple(mp))
G=sorted(set(G))
assert len(G)==24

def cycles(mp):
    seen=[False]*len(mp); lens=[]
    for i in range(len(mp)):
        if not seen[i]:
            j=i;l=0
            while not seen[j]:
                seen[j]=True;l+=1;j=mp[j]
            lens.append(l)
    return tuple(sorted(lens))

def fixed_k(cyc,k):
    dp=[0]*(k+1);dp[0]=1
    for L in cyc:
        nd=dp[:]
        for j in range(k-L+1):
            if dp[j]: nd[j+L]+=dp[j]
        dp=nd
    return dp[k]

rows=Counter()
s6=s7=0
for mp in G:
    cyc=cycles(mp)
    f6=fixed_k(cyc,6);f7=fixed_k(cyc,7)
    rows[(cyc,f6,f7)]+=1
    s6+=f6;s7+=f7
print('Aut(Q3)=',len(auts),'edge-stabilizer=',len(stab),'Gamma=',len(G))
print('Burnside sum k=6:',s6,'orbits=',s6//len(G),'remainder=',s6%len(G))
print('Burnside sum k=7:',s7,'orbits=',s7//len(G),'remainder=',s7%len(G))
print('orbit-table rows:')
for (cyc,f6,f7),mult in sorted(rows.items(), key=lambda x:(len(x[0][0]),x[0][0],x[0][1],x[0][2])):
    print('mult',mult,'cycles',','.join(map(str,cyc)),'fixed6',f6,'fixed7',f7)
