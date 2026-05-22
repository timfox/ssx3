#!/usr/bin/env python3
"""Fast .text compare for all src/mem/units; tag @objdiff-matched on match."""

from __future__ import annotations

import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
UNITS = ROOT / "src/mem/units"
TAG = "// @objdiff-matched"
EE_GCC = ROOT / "tools/cc/eegcc-2.95.3-V1.36/bin/ee-gcc2953.exe"
PATCH = ["python3", str(ROOT / "tools/patch_o32_abi.py")]
OBJCOPY = "mips-linux-gnu-objcopy"
STRIP = ["mips-linux-gnu-strip", "-N", "dummy-symbol-name"]


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
    task = "cc" if unit.suffix == ".c" else "cpp"
    flag = "-xc" if unit.suffix == ".c" else "-xc++"
    cmd = [
        "wine",
        str(EE_GCC),
        flag,
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


def tag_unit(path: Path, matched: bool) -> None:
    text = path.read_text(encoding="utf-8", errors="replace")
    if matched and TAG not in text:
        text = text.replace('#include "common.h"\n', f'#include "common.h"\n{TAG}\n', 1)
    elif not matched and TAG in text:
        text = re.sub(rf"^{re.escape(TAG)}\n", "", text, flags=re.M)
    path.write_text(text, encoding="utf-8")


def main() -> int:
    matched = 0
    total = 0
    for unit in sorted(list(UNITS.glob("*.c")) + list(UNITS.glob("*.cpp"))):
        key = unit.stem
        target = ROOT / f"obj/target/{key}.o"
        if not target.is_file():
            continue
        total += 1
        with tempfile.TemporaryDirectory() as tmp:
            cur = Path(tmp) / "cur.o"
            if not compile_unit(unit, cur):
                tag_unit(unit, False)
                print(f"[units] FAIL compile {key}")
                continue
            ok = text_bytes(target) == text_bytes(cur)
        tag_unit(unit, ok)
        print(f"[units] {'MATCH' if ok else 'diff '} {key}")
        if ok:
            matched += 1
    print(f"[units] {matched}/{total} .text matched (tagged)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
