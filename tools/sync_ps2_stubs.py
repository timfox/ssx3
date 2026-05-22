#!/usr/bin/env python3
"""Generate missing *_ps2.s stubs listed in configure.py for a given prefix/dir."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIGURE = ROOT / "configure.py"
GEN = ROOT / "tools/gen_ps2_from_splat.py"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--prefix", required=True, help="segment key prefix, e.g. dcd10_ or late_")
    ap.add_argument("--ps2-dir", required=True, help="e.g. src/dcd10")
    ap.add_argument("--asm-subpath", required=True, help="e.g. dcd10 or late")
    args = ap.parse_args()

    ps2_dir = ROOT / args.ps2_dir
    asm_dir = ROOT / "asm" / args.asm_subpath
    cfg = CONFIGURE.read_text(encoding="utf-8")
    pat = rf'"{re.escape(args.prefix)}([A-Za-z0-9_]+)": Path\("{re.escape(args.ps2_dir)}/'
    segs = sorted(set(f"{args.prefix}{m}" for m in re.findall(pat, cfg)))
    missing = [s for s in segs if not (ps2_dir / f"{s}_ps2.s").is_file()]
    if not missing:
        print(f"all {args.prefix} *_ps2.s present ({len(segs)})")
        return 0
    print(f"generating {len(missing)} missing {args.prefix} ps2 stub(s)...")
    failed = []
    for i, seg in enumerate(missing, 1):
        glabel = seg[len(args.prefix) :]
        asm = asm_dir / f"{seg}.s"
        ps2 = ps2_dir / f"{seg}_ps2.s"
        if not asm.is_file():
            failed.append(f"{seg}: no {asm}")
            continue
        r = subprocess.run(
            [sys.executable, str(GEN), str(asm), glabel, str(ps2)],
            capture_output=True,
            text=True,
        )
        if r.returncode:
            failed.append(f"{seg}: {r.stderr.strip()[:120]}")
        elif i % 200 == 0 or i == len(missing):
            print(f"  {i}/{len(missing)}")
    if failed:
        print(f"failed: {len(failed)}")
        for line in failed[:10]:
            print(f"  {line}")
        return 1
    print("done")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
