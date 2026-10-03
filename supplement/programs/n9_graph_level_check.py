#!/usr/bin/env python3
from __future__ import annotations

from itertools import combinations, permutations
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CERT = ROOT / "certificates" / "n9_certificate.json"

EDGE = lambda a, b: (a, b) if a < b else (b, a)
CUBE_EDGES = frozenset(
    EDGE(a, b)
    for a in range(8)
    for b in range(a + 1, 8)
    if (a ^ b).bit_count() == 1
)


def image_edges(p: tuple[int, ...]) -> frozenset[tuple[int, int]]:
    return frozenset(EDGE(p[a], p[b]) for a, b in CUBE_EDGES)


def enumerate_cubes(n: int) -> set[frozenset[tuple[int, int]]]:
    cubes: set[frozenset[tuple[int, int]]] = set()
    for verts in combinations(range(n), 8):
        for perm in permutations(verts):
            cubes.add(image_edges(perm))
    return cubes


def edge_set(pairs: list[list[int]]) -> set[tuple[int, int]]:
    return {EDGE(a, b) for a, b in pairs}


def main() -> None:
    cert = json.loads(CERT.read_text())
    assert cert["n"] == 9
    initial = edge_set(cert["initial_edges"])
    sequence = [EDGE(*e) for e in cert["sequence"]]
    witnesses = cert["witnesses"]

    cubes = enumerate_cubes(9)
    assert len(cubes) == 7560, len(cubes)

    initial_copies = sum(cube <= initial for cube in cubes)
    assert initial_copies == 0, initial_copies

    current = set(initial)
    for idx, (new_edge, witness) in enumerate(zip(sequence, witnesses), start=1):
        assert EDGE(*witness["edge"]) == new_edge
        cube = edge_set(witness["cube_edges"])
        assert cube in cubes, (idx, new_edge, witness["cube_edges"])
        assert new_edge in cube
        prereq = cube - {new_edge}
        assert prereq <= current, (idx, new_edge, sorted(prereq - current))
        assert new_edge not in current, (idx, new_edge)
        current.add(new_edge)

    all_edges = {EDGE(a, b) for a, b in combinations(range(9), 2)}
    assert current == all_edges

    print("copies=7560")
    print(f"initial_Q3_copies={initial_copies}")
    print(f"certificate_steps={len(sequence)}")
    print(f"final_edges={len(current)}")
    print("PASS: n=9 certificate verified against all 7,560 labelled copies of Q3 in K9.")


if __name__ == "__main__":
    main()
