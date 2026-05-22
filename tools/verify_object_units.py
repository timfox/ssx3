#!/usr/bin/env python3
"""Build obj/target + obj/current per-unit objects and tag matching C units."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
UNITS = ROOT / "src/mem/units"
TAG = "// @objdiff-matched"
OBJCOPY = "mips-linux-gnu-objcopy"


def text_bytes(path: Path) -> bytes | None:
    import tempfile

    out = Path(tempfile.mkstemp(suffix=".text")[1])
    try:
        r = subprocess.run(
            [OBJCOPY, "-O", "binary", "--only-section=.text", str(path), str(out)],
            capture_output=True,
        )
        if r.returncode != 0 or not out.is_file():
            return None
        return out.read_bytes()
    finally:
        out.unlink(missing_ok=True)


def tag_unit(path: Path, add: bool) -> None:
    text = path.read_text(encoding="utf-8", errors="replace")
    if add and TAG not in text:
        text = text.replace('#include "common.h"\n', f'#include "common.h"\n{TAG}\n', 1)
        path.write_text(text, encoding="utf-8")
    elif not add and TAG in text:
        text = re.sub(r"^// @objdiff-matched\n", "", text, flags=re.M)
        path.write_text(text, encoding="utf-8")


def main() -> int:
    if not UNITS.is_dir():
        print("[units] no src/mem/units/")
        return 0

    subprocess.run(
        [str(ROOT / "venv/bin/python"), str(ROOT / "configure.py"), "-o"],
        cwd=ROOT,
        check=True,
    )
    subprocess.run(
        ["python3", str(ROOT / "tools/extract_object_c_units.py")],
        cwd=ROOT,
        check=True,
    )

    matched = 0
    total = 0
    for unit in sorted(list(UNITS.glob("*.cpp")) + list(UNITS.glob("*.c"))):
        key = unit.stem
        target = ROOT / f"obj/target/{key}.o"
        current = ROOT / f"obj/current/{key}.o"
        if not target.is_file():
            continue
        total += 1
        subprocess.run(
            ["ninja", str(target), str(current)],
            cwd=ROOT,
            check=False,
        )
        if not current.is_file():
            print(f"[units] missing {current.name}")
            continue
        tgt_text = text_bytes(target)
        cur_text = text_bytes(current)
        ok = (
            tgt_text is not None
            and cur_text is not None
            and tgt_text == cur_text
        )
        if ok:
            tag_unit(unit, True)
            print(f"[units] MATCH {key} (.text)")
            matched += 1
        else:
            tag_unit(unit, False)
            print(f"[units] diff  {key}")

    print(f"[units] {matched}/{total} byte-identical")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
