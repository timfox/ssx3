#!/usr/bin/env python3
"""List tiny *_ps2.s stubs still using asm embed (not readable EE C).

Jr-delay (jr $ra; nop) stubs are handled by tools/auto_try_jr_delay_units.py.
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MEM = ROOT / "src/mem"
UNITS = ROOT / "src/mem/units"
TARGET = ROOT / "obj/target"

WORD_RE = re.compile(r"\.word\s+0x([0-9A-Fa-f]+)")


def classify(words: list[int]) -> str:
    if len(words) == 1 and words[0] == 0:
        return "nop-only"
    if len(words) == 2 and words[0] == 0x03E00008 and words[1] == 0:
        return "jr-delay-nop"
    if len(words) == 2 and words[0] == 0x03E00008 and (words[1] >> 16) == 0x2402:
        return "jr-delay-return"
    if len(words) == 2 and words[0] == 0x03E00008:
        return "jr-delay-other"
    if len(words) == 2 and words[1] == 0x03E00008:
        return "delay-jr"
    if (
        len(words) == 2
        and words[0] == 0x03E00008
        and (words[1] & 0xFFFF0000) == 0xACA00000
    ):
        return "back-link"
    return "other"


def main() -> int:
    patterns: dict[str, list[str]] = {}
    for ps2 in sorted(MEM.glob("rom21_*_ps2.s")):
        key = ps2.stem.replace("_ps2", "")
        if not (TARGET / f"{key}.o").is_file():
            continue
        words = [int(w, 16) for w in WORD_RE.findall(ps2.read_text(encoding="utf-8", errors="replace"))]
        if len(words) > 4:
            continue
        unit = UNITS / f"{key}.c"
        if unit.is_file() and "PS2_RETAIL_ASM_ONLY" not in unit.read_text(
            encoding="utf-8", errors="replace"
        ):
            continue
        patterns.setdefault(classify(words), []).append(key)

    print("Tiny PS2 stubs (<=4 words) without readable C unit:\n")
    for kind in (
        "back-link",
        "jr-delay-nop",
        "jr-delay-return",
        "jr-delay-other",
        "delay-jr",
        "nop-only",
        "other",
    ):
        items = patterns.get(kind, [])
        if not items:
            continue
        print(f"## {kind} ({len(items)})")
        for key in items[:12]:
            print(f"  {key}")
        if len(items) > 12:
            print(f"  ... +{len(items) - 12} more")
        print()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
