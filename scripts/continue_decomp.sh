#!/usr/bin/env bash
# Mechanical decomp pipeline: drain remaining monoliths and report progress.
set -euo pipefail
cd "$(dirname "$0")/.."

if grep -q '1218_tail' config/ssx3_us.yaml 2>/dev/null || grep -qi '\[0x1218, asm\]' config/ssx3_us.yaml; then
  echo "=== draining early 0x1218 monolith ==="
  python3 tools/split_1218_tail.py --batch 100 --until-empty
fi

if grep -qi '\[0x1dcd10, asm\]' config/ssx3_us.yaml; then
  echo "=== draining 0x1dcd10 monolith (935 funcs) ==="
  python3 tools/split_monolith.py --asm asm/1DCD10.s --tail-name 1dcd10_tail \
    --prefix dcd10_ --region-start 0x1dcd10 --region-end 0x218618 \
    --yaml-subpath dcd10 --ps2-dir src/dcd10 \
    --configure-anchor crowd_anim2d_update --batch 100 --until-empty
fi

if grep -qi '\[0x2eef50, asm\]' config/ssx3_us.yaml; then
  echo "=== draining 0x2eef50 late monolith ==="
  python3 tools/split_monolith.py --asm asm/2EEF50.s --tail-name 2eef50_tail \
    --prefix late_ --region-start 0x2eef50 --region-end 0x32f590 \
    --yaml-subpath late --ps2-dir src/late \
    --configure-anchor rom21_func_003EB190 --batch 100 --until-empty
fi

if grep -q 'rom_21e5a8_tail' config/ssx3_us.yaml 2>/dev/null; then
  echo "=== draining rom21 tail ==="
  python3 tools/split_rom21_tail.py --batch 100 --until-empty
fi

python3 tools/sync_rom21_ps2.py 2>/dev/null || true
python3 tools/sync_early_ps2.py 2>/dev/null || true
python3 tools/sync_ps2_stubs.py --prefix dcd10_ --ps2-dir src/dcd10 --asm-subpath dcd10 2>/dev/null || true
python3 tools/sync_ps2_stubs.py --prefix late_ --ps2-dir src/late --asm-subpath late 2>/dev/null || true
python3 tools/extract_object_c_units.py 2>/dev/null || true
venv/bin/python configure.py -o 2>/dev/null || true
python3 tools/batch_verify_units.py 2>/dev/null || true
python3 scripts/sync_native_decomp.py 2>/dev/null || true
python3 tools/decomp_progress.py
