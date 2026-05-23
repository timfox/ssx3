#!/usr/bin/env python3
"""Readable EE C for jr $ra + addiu $v0,$zero,imm (return small constant)."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MEM = ROOT / "src/mem"
UNITS = ROOT / "src/mem/units"
TARGET = ROOT / "obj/target"
TAG = "// @objdiff-matched"
EE_GCC = ROOT / "tools/cc/eegcc-2.95.3-V1.36/bin/ee-gcc2953.exe"
PATCH = ["python3", str(ROOT / "tools/patch_o32_abi.py")]
OBJCOPY = "mips-linux-gnu-objcopy"
STRIP = ["mips-linux-gnu-strip", "-N", "dummy-symbol-name"]
WORD_RE = re.compile(r"\.word\s+0x([0-9A-Fa-f]+)")

sys.path.insert(0, str(ROOT / "tools"))
from ps2_s_to_retail_asm import symbol_from_ps2  # noqa: E402


def imm16_signed(word: int) -> int:
    imm = word & 0xFFFF
    return imm - 0x10000 if imm >= 0x8000 else imm


def is_return_const(words: list[int]) -> bool:
    return len(words) == 2 and words[0] == 0x03E00008 and (words[1] >> 16) == 0x2402


def text_bytes(path: Path) -> bytes | None:
    out = Path(tempfile.mkstemp(suffix=".text")[1])
    try:
        r = subprocess.run(
            [OBJCOPY, "-O", "binary", "--only-section=.text", str(path), str(out)],
            capture_output=True,
        )
        if r.returncode != 0:
            return None
        return out.read_bytes()
    finally:
        out.unlink(missing_ok=True)


def compile_unit(unit: Path, out_o: Path) -> bool:
    cmd = [
        "wine",
        str(EE_GCC),
        "-xc",
        "-c",
        "-Iinclude",
        "-isystem",
        "include/sdk/ee",
        "-B",
        str(ROOT / "tools/cc/eegcc-2.95.3-V1.36/lib/gcc-lib/ee/2.95.3/"),
        "-O2",
        str(unit),
        "-DSKIP_ASM",
        "-o",
        str(out_o),
    ]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True)
    if r.returncode != 0:
        return False
    subprocess.run([*PATCH, str(out_o)], check=True)
    subprocess.run([*STRIP, str(out_o)], check=False)
    return True


def unit_source(sym: str, ret: int) -> str:
    return (
        '#include "common.h"\n'
        '#include "ps2_match.h"\n\n'
        "#ifdef SKIP_ASM\n"
        f"int {sym}(void) {{ return {ret}; }}\n"
        "#endif\n"
    )


def tag_unit(path: Path, matched: bool) -> None:
    text = path.read_text(encoding="utf-8", errors="replace")
    if matched and TAG not in text:
        text = text.replace('#include "common.h"\n', f'#include "common.h"\n{TAG}\n', 1)
    elif not matched and TAG in text:
        text = re.sub(rf"^{re.escape(TAG)}\n", "", text, flags=re.M)
    path.write_text(text, encoding="utf-8")


def candidates() -> list[tuple[str, int]]:
    out: list[tuple[str, int]] = []
    for ps2 in sorted(MEM.glob("*_ps2.s")):
        words = [
            int(w, 16)
            for w in WORD_RE.findall(ps2.read_text(encoding="utf-8", errors="replace"))
        ]
        if not is_return_const(words):
            continue
        key = ps2.stem.replace("_ps2", "")
        if not (TARGET / f"{key}.o").is_file():
            continue
        unit = UNITS / f"{key}.c"
        if unit.is_file() and "PS2_RETAIL_ASM_ONLY" not in unit.read_text(
            encoding="utf-8", errors="replace"
        ):
            continue
        out.append((key, imm16_signed(words[1])))
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    items = candidates()
    if args.limit:
        items = items[: args.limit]

    matched = failed = 0
    for key, ret in items:
        sym = symbol_from_ps2(MEM / f"{key}_ps2.s")
        unit = UNITS / f"{key}.c"
        if args.dry_run:
            print(f"{key} {sym} return {ret}")
            continue
        unit.write_text(unit_source(sym, ret), encoding="utf-8")
        target = TARGET / f"{key}.o"
        with tempfile.TemporaryDirectory() as tmp:
            cur_o = Path(tmp) / "cur.o"
            if not compile_unit(unit, cur_o):
                print(f"FAIL compile {key}")
                failed += 1
                continue
            ok = text_bytes(cur_o) == text_bytes(target)
        tag_unit(unit, ok)
        if ok:
            matched += 1
            print(f"OK {key} return {ret}")
        else:
            failed += 1
            print(f"DIFF {key} return {ret}")

    if not args.dry_run:
        print(f"\n{matched} matched, {failed} failed, {len(items)} tried")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
