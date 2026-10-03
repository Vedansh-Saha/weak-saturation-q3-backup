# Reproducibility record

The computations in the manuscript are finite exact enumerations and certificate checks.

## Toolchain

The verification environment recorded for the distributed results is listed in `logs/final_verification/toolchain.txt`.

## Exact objects

The mathematical objects tested by the programs are specified in `PROOF_SPEC.md`. The machine-readable certificates are in `supplement/certificates/`.

## Lower-bound statements

For n=9, all 10,626 four-edge seeds are tested directly. A second implementation in Go repeats the four-edge exclusion.

For n=10, the exact six-edge layer is reduced by the stabilizer of the marked cube edge, giving 146,131 orbit representatives.

For n=11, the complete six-edge orbit layer contains 300,563 representatives. Adjoining one edge to each representative gives 11,120,831 candidates before deduplication and 1,498,208 seven-edge representatives after canonicalization. Burnside's lemma independently reproduces the two orbit counts.

## Upper-bound statements

Every certificate row supplies a new edge and an explicit cube witness. The witness-table checker verifies injectivity, the target edge, and the presence of the remaining eleven cube edges at the required stage.

## Integrity

`SHA256SUMS.txt` records SHA-256 hashes for the distributed files in the research package.
