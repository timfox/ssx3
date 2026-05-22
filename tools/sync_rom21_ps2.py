#!/usr/bin/env python3
"""Generate missing src/mem/rom21_*_ps2.s from asm/mem/rom21_*.s (configure ps2_asm_map)."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIGURE = ROOT / "configure.py"
ASM_DIR = ROOT / "asm/mem"
PS2_DIR = ROOT / "src/mem"
GEN = ROOT / "tools/gen_ps2_from_splat.py"


def segments_from_configure() -> list[str]:
    text = CONFIGURE.read_text(encoding="utf-8")
    return sorted(
        set(re.findall(r'"(rom21_[A-Za-z0-9_]+)": Path\("src/mem/rom21_[A-Za-z0-9_]+_ps2\.s"\)', text))
    )


def glabel_from_segment(seg: str) -> str:
    if seg.startswith("rom21_"):
        return seg[6:]
    return seg


def main() -> int:
    missing: list[str] = []
    for seg in segments_from_configure():
        ps2 = PS2_DIR / f"{seg}_ps2.s"
        if not ps2.is_file():
            missing.append(seg)

    if not missing:
        print("all rom21 *_ps2.s present")
        return 0

    print(f"generating {len(missing)} missing ps2 stub(s)...")
    failed: list[str] = []
    for i, seg in enumerate(missing, 1):
        glabel = glabel_from_segment(seg)
        asm = ASM_DIR / f"{seg}.s"
        ps2 = PS2_DIR / f"{seg}_ps2.s"
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
        for line in failed[:15]:
            print(f"  {line}")
        return 1
    print("done")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
