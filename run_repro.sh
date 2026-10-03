#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD="$ROOT/repro_build"
LOG="$ROOT/logs/repro"
rm -rf "$BUILD" "$LOG"
mkdir -p "$BUILD" "$LOG"

run_logged() {
  local name="$1"; shift
  echo "[run] $name"
  "$@" >"$LOG/${name}.stdout" 2>"$LOG/${name}.stderr"
}

cd "$ROOT"

echo "toolchain" >"$LOG/toolchain.txt"
{
  echo "g++: $(g++ --version | head -1)"
  echo "go: $(go version)"
  echo "python: $(python3 --version)"
  echo "pdflatex: $(pdflatex --version | head -1)"
  echo "kernel: $(uname -srmo)"
  echo "cpu_model: $(lscpu | sed -n 's/^Model name:[[:space:]]*//p' | head -1)"
  echo "online_cpus: $(nproc)"
} >>"$LOG/toolchain.txt"

run_logged pdf_build ./build_paper.sh

g++ -O3 -std=c++17 -fopenmp supplement/programs/n8_sanity.cpp -o "$BUILD/n8_sanity"
g++ -O3 -std=c++17 supplement/programs/n9_exact_verifier.cpp -o "$BUILD/n9_exact_verifier"
g++ -O3 -std=c++17 -fopenmp supplement/programs/n10_lower_orbit_recheck.cpp -o "$BUILD/n10_lower_orbit_recheck"
g++ -O3 -std=c++17 -fopenmp supplement/programs/n11_lower_orbit7.cpp -o "$BUILD/n11_lower_orbit7"
g++ -O3 -std=c++17 -fopenmp supplement/programs/n10_upper_verifier.cpp -o "$BUILD/n10_upper_verifier"
g++ -O3 -std=c++17 -fopenmp supplement/programs/n11_upper_verifier.cpp -o "$BUILD/n11_upper_verifier"
go build -o "$BUILD/n9_independent_lower" supplement/programs/n9_independent_lower.go

run_logged n8_sanity "$BUILD/n8_sanity"
run_logged n9_exact_verifier "$BUILD/n9_exact_verifier"
run_logged n9_independent_lower "$BUILD/n9_independent_lower"
run_logged n9_graph_level_check python3 supplement/programs/n9_graph_level_check.py
run_logged verify_witness_tables python3 supplement/programs/verify_witness_tables.py
run_logged n10_lower_orbit_recheck "$BUILD/n10_lower_orbit_recheck"
run_logged n10_upper_verifier_cpp "$BUILD/n10_upper_verifier"
run_logged n10_upper_verifier_python python3 supplement/programs/n10_upper_verifier.py
run_logged n11_upper_verifier_cpp "$BUILD/n11_upper_verifier"
run_logged n11_upper_verifier_python python3 supplement/programs/n11_upper_verifier.py
run_logged n11_burnside python3 supplement/programs/n11_burnside.py
run_logged independent_sanity python3 supplement/programs/independent_sanity.py
run_logged structural_g10_analysis python3 supplement/programs/structural_g10_analysis.py

# The n=11 lower-bound computation writes the complete seven-edge orbit layer.
cd "$ROOT/supplement"
OMP_NUM_THREADS="${OMP_NUM_THREADS:-4}" "$BUILD/n11_lower_orbit7" >"$LOG/n11_lower_orbit7.stdout" 2>"$LOG/n11_lower_orbit7.stderr"

cd "$ROOT"
rm -rf "$BUILD"
echo "All reproducibility commands completed successfully."
