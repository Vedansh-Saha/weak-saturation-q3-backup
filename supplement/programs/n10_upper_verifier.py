#!/usr/bin/env python3

from itertools import combinations, permutations

N = 10

PAIRS = [(i, j) for i in range(N) for j in range(i + 1, N)]
EID = {e: k for k, e in enumerate(PAIRS)}

Q3_EDGES = []
for a in range(8):
    for bit in (1, 2, 4):
        b = a ^ bit
        if a < b:
            Q3_EDGES.append((a, b))

LOCAL_PAIRS = [(i, j) for i in range(8) for j in range(i + 1, 8)]
LOCAL_EID = {e: k for k, e in enumerate(LOCAL_PAIRS)}
BASE = set()
for p in permutations(range(8)):
    mask = 0
    for a, b in Q3_EDGES:
        x, y = sorted((p[a], p[b]))
        mask |= 1 << LOCAL_EID[(x, y)]
    BASE.add(mask)

assert len(BASE) == 840, len(BASE)

def make_cube_masks():
    masks = set()
    for S in combinations(range(N), 8):
        trans = {}
        for (a, b), lid in LOCAL_EID.items():
            trans[lid] = EID[(min(S[a], S[b]), max(S[a], S[b]))]
        for bm in BASE:
            gm = 0
            x = bm
            while x:
                bit = x & -x
                lid = bit.bit_length() - 1
                gm |= 1 << trans[lid]
                x -= bit
            masks.add(gm)
    return sorted(masks)

CUBES = make_cube_masks()
assert len(CUBES) == 37800, len(CUBES)

def edge_mask(edges):
    m = 0
    for e in edges:
        e = tuple(sorted(e))
        m |= 1 << EID[e]
    return m

INITIAL = edge_mask([
    (0,2),(0,4),(1,2),(1,3),(1,5),(1,9),
    (2,3),(2,6),(2,9),(3,5),(3,6),(3,7),
    (4,5),(4,6),(5,7),(6,7),(6,8),(7,8),
])

ADDITION_SEQUENCE = [
    (0,1),(3,9),(0,7),(1,7),(0,3),(2,5),(4,7),(1,6),(2,7),
    (5,6),(2,4),(3,4),(0,5),(0,6),(1,4),(5,9),(4,9),(6,9),
    (0,9),(7,9),(2,8),(5,8),(1,8),(3,8),(4,8),(0,8),(8,9),
]

def bit(e):
    return 1 << EID[tuple(sorted(e))]

def verify():
    assert INITIAL.bit_count() == 18

    initial_cubes = [c for c in CUBES if (c & ~INITIAL) == 0]
    assert not initial_cubes, f"Initial graph contains {len(initial_cubes)} Q3 copies."

    g = INITIAL

    for step, e in enumerate(ADDITION_SEQUENCE, start=1):
        eb = bit(e)
        assert not (g & eb), f"Step {step}: edge {e} was already present."

        witnesses = []
        for c in CUBES:
            if not (c & eb):
                continue
            if (c & ~g) == eb:
                witnesses.append(c)

        assert witnesses, f"Step {step}: {e} does not complete a Q3."
        g |= eb

    assert len(ADDITION_SEQUENCE) == 27
    assert g.bit_count() == 45
    assert g == (1 << 45) - 1

    print("PASS")
    print("Initial edges:", INITIAL.bit_count())
    print("Added edges:", len(ADDITION_SEQUENCE))
    print("Final edges:", g.bit_count())
    print("Initial graph is Q3-free.")
    print("Every added edge completes a Q3.")
    print("Therefore wsat(10,Q3) <= 18.")

if __name__ == "__main__":
    verify()
