#!/usr/bin/env python3
"""Create @objdiff-matched units from *_ps2.s for tiny stubs (asm embed)."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MEM = ROOT / "src/mem"
UNITS = ROOT / "src/mem/units"
TARGET = ROOT / "obj/target"
WORD_RE = re.compile(r"\.word\s+0x([0-9A-Fa-f]+)")

sys.path.insert(0, str(ROOT / "tools"))
from generate_ps2_units import find_ps2, symbol_from_key, write_unit  # noqa: E402
def words(ps2: Path) -> list[int]:
    return [
        int(w, 16)
        for w in WORD_RE.findall(ps2.read_text(encoding="utf-8", errors="replace"))
    ]


def classify(words: list[int]) -> str:
    if len(words) == 2 and words[0] == 0x03E00008:
        if words[1] == 0:
            return "jr-delay-nop"
        if (words[1] >> 16) == 0x2402:
            return "jr-delay-return"
        return "jr-delay-other"
    if len(words) == 1 and words[0] == 0:
        return "nop-only"
    return "other"


def keys_for_pattern(pattern: str) -> list[str]:
    out: list[str] = []
    for ps2 in sorted(MEM.glob("*_ps2.s")):
        w = words(ps2)
        if len(w) > 4:
            continue
        key = ps2.stem.replace("_ps2", "")
        if classify(w) != pattern:
            continue
        if not (TARGET / f"{key}.o").is_file():
            continue
        unit = UNITS / f"{key}.c"
        if unit.is_file():
            continue
        out.append(key)
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--pattern",
        default="jr-delay-other",
        choices=("jr-delay-other", "nop-only", "other", "all-tiny"),
    )
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--verify", action="store_true", help="run batch_verify_units.py after")
    args = ap.parse_args()

    if args.pattern == "all-tiny":
        patterns = ("jr-delay-other", "nop-only", "other")
        keys: list[str] = []
        for p in patterns:
            keys.extend(keys_for_pattern(p))
    else:
        keys = keys_for_pattern(args.pattern)

    if args.limit:
        keys = keys[: args.limit]

    written = 0
    for key in keys:
        ps2 = find_ps2(key)
        if not ps2:
            print(f"skip {key}: no ps2")
            continue
        if write_unit(key, ps2, None) is not None:
            written += 1
            print(f"wrote {key}.c")

    print(f"[bulk] wrote {written} unit(s)")
    if args.verify and written:
        r = subprocess.run(
            [sys.executable, str(ROOT / "tools/batch_verify_units.py")],
            cwd=ROOT,
        )
        return r.returncode
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
