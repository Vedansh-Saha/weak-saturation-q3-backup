#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
OUT="$ROOT/paper"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$OUT"
cd "$ROOT/source"
pdflatex -interaction=nonstopmode -halt-on-error -output-directory="$OUT" wsat_q3.tex >"$TMP/pass1.log" 2>&1
pdflatex -interaction=nonstopmode -halt-on-error -output-directory="$OUT" wsat_q3.tex >"$TMP/pass2.log" 2>&1
if grep -Eq '^(!|LaTeX Warning|Package .* Warning|pdfTeX warning)' "$TMP/pass2.log"; then
  cat "$TMP/pass2.log"
  exit 1
fi
rm -f "$OUT/wsat_q3.aux" "$OUT/wsat_q3.log" "$OUT/wsat_q3.out"
echo "Built $OUT/wsat_q3.pdf"
