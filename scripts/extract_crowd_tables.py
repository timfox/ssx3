#!/usr/bin/env python3
"""Extract retail crowd anim tables from SLUS_207.72 into crowdrender2d_data.cpp."""

from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ELF = ROOT / "disc" / "SLUS_207.72"
OUT = ROOT / "src" / "visualfx" / "crowdrender2d_data.cpp"
DATA_VA = 0x43CE00
DATA_OFF = 0x33DE00

# cCrowdRender2D_init indexes 16 variants @ +0xd8 and writes shape slots through ~0xd28;
# 0x900 was too small and caused host heap corruption after init.
TABLES = (
    ("D_00445D38", 0x445D38, 0x1000),
    ("D_00445E10", 0x445E10, 0x80),
    ("D_00445E20", 0x445E20, 0x20),
)


def extract(elf: bytes, va: int, size: int) -> bytes:
    off = DATA_OFF + (va - DATA_VA)
    return elf[off : off + size]


def emit_array(sym: str, va: int, data: bytes) -> list[str]:
    lines = [
        f"/* Retail {sym} @ {va:#010x} ({len(data)} bytes) */",
        f"unsigned char {sym}[] = {{",
    ]
    for i in range(0, len(data), 12):
        chunk = data[i : i + 12]
        hexes = ", ".join(f"0x{b:02x}" for b in chunk)
        lines.append(f"    {hexes},")
    lines.append("};")
    return lines


def main() -> None:
    if not ELF.is_file():
        raise SystemExit(f"missing {ELF}")

    elf = ELF.read_bytes()
    parts = ['#include "common.h"', "", "#if defined(SSX3_HOST) || defined(SKIP_ASM)", ""]
    for sym, va, size in TABLES:
        parts.extend(emit_array(sym, va, extract(elf, va, size)))
        parts.append("")
    parts.append("#endif")
    OUT.write_text("\n".join(parts) + "\n", encoding="utf-8")
    print(f"wrote {OUT}")


if __name__ == "__main__":
    main()
