#!/usr/bin/env python3
from __future__ import annotations

import csv
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CANONICAL_Q3 = {
    tuple(sorted(e)) for e in (
        (0,1),(0,2),(0,4),(1,3),(1,5),(2,3),
        (2,6),(3,7),(4,5),(4,6),(5,7),(6,7)
    )
}

def E(a: int, b: int) -> tuple[int, int]:
    return (a, b) if a < b else (b, a)

def edge_set(rows: list[list[int]]) -> set[tuple[int, int]]:
    return {E(a, b) for a, b in rows}

def parse_edge(s: str) -> tuple[int, int]:
    a, b = int(s[0]), int(s[1:])
    return E(a, b)

def main() -> None:
    for n in (9, 10, 11):
        cert_path = ROOT / "certificates" / f"n{n}_certificate.json"
        table_path = ROOT / "certificates" / f"n{n}_witness_table.tsv"
        cert = json.loads(cert_path.read_text())
        initial = edge_set(cert["initial_edges"])
        sequence = [E(*e) for e in cert["sequence"]]
        witnesses = cert["witnesses"]

        with table_path.open(newline="") as fh:
            rows = list(csv.DictReader(fh, delimiter="\t"))
        assert len(rows) == len(sequence) == len(witnesses)
        current = set(initial)

        for i, (target, witness, row) in enumerate(zip(sequence, witnesses, rows), start=1):
            assert int(row["step"]) == i
            assert parse_edge(row["edge"]) == target
            phi = [int(x) for x in row["phi(0..7)"].split(",")]
            assert len(phi) == 8 and len(set(phi)) == 8, (n, i, phi)
            mapped = {E(phi[a], phi[b]) for a, b in CANONICAL_Q3}
            stored_cube = edge_set(witness["cube_edges"])
            assert mapped == stored_cube, (n, i, "vertex map does not produce stored cube")
            assert E(phi[0], phi[1]) == target, (n, i, "target mismatch")

            prereq = stored_cube - {target}
            assert target not in current, (n, i, "target already present")
            assert prereq <= current, (n, i, sorted(prereq-current))

            listed = set()
            dep_text = row["newly_added_witness_steps"].strip()
            if dep_text != "-":
                listed = {int(x) for x in dep_text.split(",")}
            expected = {j for j, e in enumerate(sequence[:i-1], start=1) if e in prereq}
            assert listed == expected, (n, i, listed, expected)

            json_prereq = edge_set(witness["prerequisites"])
            assert json_prereq == prereq, (n, i, "stored prerequisites mismatch")
            current.add(target)

        all_edges = {E(i, j) for i in range(n) for j in range(i+1, n)}
        assert current == all_edges, (n, "final graph incomplete")
        print(f"n={n}: {len(rows)} witness rows PASS")
    print("PASS: all vertex maps, cube images, prerequisites, dependencies, and final graphs verified.")

if __name__ == "__main__":
    main()
