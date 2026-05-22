#!/usr/bin/env python3
"""Generate missing src/early/*_ps2.s from asm/early/*.s (configure ps2_asm_map)."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIGURE = ROOT / "configure.py"
GEN = ROOT / "tools/gen_ps2_from_splat.py"


def main() -> int:
    cfg = CONFIGURE.read_text(encoding="utf-8")
    segs = sorted(set(re.findall(r'"(early_[A-Za-z0-9_]+)": Path\("src/early/', cfg)))
    missing = [s for s in segs if not (ROOT / "src/early" / f"{s}_ps2.s").is_file()]
    if not missing:
        print("all early *_ps2.s present")
        return 0
    print(f"generating {len(missing)} missing early ps2 stub(s)...")
    failed = []
    for i, seg in enumerate(missing, 1):
        glabel = seg[6:] if seg.startswith("early_") else seg
        asm = ROOT / "asm/early" / f"{seg}.s"
        ps2 = ROOT / "src/early" / f"{seg}_ps2.s"
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
