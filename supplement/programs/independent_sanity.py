#!/usr/bin/env python3
from itertools import combinations, permutations
from pathlib import Path
import json

ROOT = Path(__file__).resolve().parents[2]
CERT_DIR = ROOT / "supplement" / "certificates"

Q3 = {tuple(sorted(e)) for e in (
    (0,1),(0,2),(0,4),(1,3),(1,5),(2,3),
    (2,6),(3,7),(4,5),(4,6),(5,7),(6,7)
)}
LOCAL_EDGES = [(i, j) for i in range(8) for j in range(i + 1, 8)]
LOCAL_ID = {e: i for i, e in enumerate(LOCAL_EDGES)}

BASE_CUBES = set()
for p in permutations(range(8)):
    m = 0
    for a, b in Q3:
        x, y = sorted((p[a], p[b]))
        m |= 1 << LOCAL_ID[(x, y)]
    BASE_CUBES.add(m)
assert len(BASE_CUBES) == 840


def labelled_cubes(n: int):
    edges = [(i, j) for i in range(n) for j in range(i + 1, n)]
    eid = {e: i for i, e in enumerate(edges)}
    cubes = set()
    for V in combinations(range(n), 8):
        for bm in BASE_CUBES:
            m = 0
            for a, b in LOCAL_EDGES:
                if (bm >> LOCAL_ID[(a, b)]) & 1:
                    x, y = sorted((V[a], V[b]))
                    m |= 1 << eid[(x, y)]
            cubes.add(m)
    expected = 840 * __import__('math').comb(n, 8)
    assert len(cubes) == expected, (n, len(cubes), expected)
    return edges, eid, cubes


def certificate_mask(es, eid):
    m = 0
    for a, b in es:
        x, y = sorted((a, b))
        m |= 1 << eid[(x, y)]
    return m


def verify_certificate(path: Path):
    d = json.loads(path.read_text())
    n = d["n"]
    edges, eid, cubes = labelled_cubes(n)
    all_mask = (1 << len(edges)) - 1
    state = certificate_mask(d["initial_edges"], eid)
    initial_cubes = [c for c in cubes if (c & ~state) == 0]
    assert not initial_cubes, f"{path.name}: initial graph contains Q3"
    assert len(d["sequence"]) == len(edges) - len(d["initial_edges"])
    assert len(d["witnesses"]) == len(d["sequence"])

    for expected_edge, witness in zip(d["sequence"], d["witnesses"]):
        target = tuple(expected_edge)
        assert tuple(witness["edge"]) == target, (path.name, expected_edge, witness["edge"])
        ebit = certificate_mask([target], eid)
        assert not (state & ebit), f"{path.name}: step {witness['step']} repeats an edge"
        wcube = certificate_mask(witness["cube_edges"], eid)
        assert wcube & ebit, f"{path.name}: witness misses target at step {witness['step']}"
        assert (wcube & ~state) == ebit, f"{path.name}: illegal witness at step {witness['step']}"
        assert wcube in cubes, f"{path.name}: witness is not Q3 at step {witness['step']}"
        state |= ebit

    assert state == all_mask, f"{path.name}: terminal graph is not Kn"
    return len(cubes)


def verify_graph6(n, initial_edges, expected):
    try:
        import networkx as nx
    except ImportError:
        return "SKIP (networkx unavailable)"
    G = nx.Graph()
    G.add_nodes_from(range(n))
    G.add_edges_from(tuple(e) for e in initial_edges)
    got = nx.to_graph6_bytes(G, header=False).decode().strip()
    assert got == expected, (n, got, expected)
    return "PASS"


expected_graph6 = {
    9: "H^bHOm_",
    10: "IZ`XokBW?",
    11: "JR`XOkY?Ga_",
}

for n in (9, 10, 11):
    path = CERT_DIR / f"n{n}_certificate.json"
    d = json.loads(path.read_text())
    cubes = verify_certificate(path)
    print(path.name, "PASS", "distinct_cubes=", cubes,
          "graph6=", verify_graph6(n, d["initial_edges"], expected_graph6[n]))

switches = [
    Q3 - {(0,1),(6,7)} | {(0,7),(1,6)},
    Q3 - {(0,2),(5,7)} | {(0,7),(2,5)},
    Q3 - {(0,4),(3,7)} | {(0,7),(3,4)},
]
for idx, cube in enumerate(switches, 1):
    for p in permutations(range(8)):
        mapped = {tuple(sorted((p[a], p[b]))) for a, b in Q3}
        if mapped == cube:
            print("switch", idx, "PASS", p)
            break
    else:
        raise AssertionError(f"switch {idx} is not isomorphic to Q3")

print("ALL INDEPENDENT SANITY CHECKS PASS")
