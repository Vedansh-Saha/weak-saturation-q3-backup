# Weak Saturation of the 3-Cube in Complete Graphs

This repository contains the source code, certificates, verification records, and LaTeX source accompanying

**Weak Saturation of the 3-Cube in Complete Graphs: Exact Values at Orders 9,10,11**.

The manuscript establishes

$$\text{wsat}(K_9, Q_3) = 16, \quad \text{wsat}(K_{10}, Q_3) = 18, \quad \text{wsat}(K_{11}, Q_3) = 19$$

and

$$\text{wsat}(K_n, Q_3) \le 2n - 3 \quad (n \ge 11)$$

## Repository contents

```text
paper/                      Compiled manuscript
source/                     LaTeX source, figures, and witness tables
supplement/certificates/    Machine-readable and human-readable certificates
supplement/programs/        Exact searches and verification programs
supplement/n11_reps7.bin   Complete seven-edge orbit-representative layer for n=11
supplement/structural_g10_analysis.txt
logs/final_verification/    Recorded verification outputs and toolchain information
build_paper.sh              Two-pass manuscript build
run_repro.sh                Reproduction script
PROOF_SPEC.md               Finite verification specification
CITATION.cff                Citation metadata
SHA256SUMS.txt              File checksums
```

The `audit/` material used for local PDF inspection is intentionally omitted from the repository. It is not required to reproduce the mathematical computations or the manuscript.

## Reproducing the computations

The commands are

```bash
./build_paper.sh
./run_repro.sh
```

The full finite searches for the lower bounds at orders 10 and 11 are the longest computations. The recorded outputs in `logs/final_verification/` correspond to the version of the programs distributed with this repository.

The verification programs include:

- `n9_exact_verifier.cpp`: exact four-edge exclusion and five-edge existence check for n=9, together with certificate verification;
- `n9_independent_lower.go`: independent n=9 lower-bound computation using explicit edge lists and a queue-based closure routine;
- `n9_graph_level_check.py`: graph-level verification against all 7,560 labelled copies of Q3 in K9;
- `n10_lower_orbit_recheck.cpp`: symmetry-reduced exact lower-bound computation for n=10;
- `n11_lower_orbit7.cpp`: symmetry-reduced exact lower-bound computation for n=11;
- `n11_burnside.py`: independent Burnside computation of the orbit counts;
- `n10_upper_verifier.{cpp,py}` and `n11_upper_verifier.{cpp,py}`: independent certificate checks;
- `verify_witness_tables.py`: verification of every witness-map row in the three certificates.

## Versioning

The release corresponding to the manuscript should be tagged in Git as `v1.0.0-paper` and archived through Zenodo. The DOI assigned to that archived release should be used in the manuscript's code-availability statement and bibliography.

## Citation

Use the citation information in `CITATION.cff`. Once the paper has a DOI, the repository citation metadata should be updated so that the preferred citation is the published article.
