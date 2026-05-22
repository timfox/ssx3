#!/usr/bin/env python3
"""Wrap #ifdef SKIP_ASM bodies with PS2_RETAIL_ASM_ONLY from matching *_ps2.s stubs."""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from ps2_s_to_retail_asm import format_word_block, words_from_ps2  # noqa: E402

MEM = ROOT / "src/mem"
OBJECT = ROOT / "src/object"

SKIP_RE = re.compile(
    r"(//[^\n]*\n)?INCLUDE_ASM\([^\)]*,\s*(\w+)\);\n#ifdef SKIP_ASM\n",
    re.M,
)


def ps2_block(sym: str) -> str | None:
    matches = list(MEM.glob(f"rom21_*{sym}*_ps2.s"))
    if not matches:
        matches = [MEM / f"rom21_{sym}_ps2.s"]
    path = matches[0]
    if not path.is_file():
        return None
    words = words_from_ps2(path)
    insn = format_word_block(words).rstrip()
    return f"#ifndef SSX3_HOST\nPS2_RETAIL_ASM_ONLY(\n    {sym},\n{insn}\n)\n#else\n"


def wrap_file(path: Path, symbols: set[str]) -> int:
    text = path.read_text(encoding="utf-8", errors="replace")
    if "#include \"ps2_match.h\"" not in text:
        text = text.replace(
            '#include "common.h"\n',
            '#include "common.h"\n#include "ps2_match.h"\n',
            1,
        )
    changed = 0
    pos = 0
    out: list[str] = []
    while True:
        m = SKIP_RE.search(text, pos)
        if not m:
            out.append(text[pos:])
            break
        sym = m.group(2)
        out.append(text[pos : m.end()])
        pos = m.end()
        if sym not in symbols:
            continue
        block = ps2_block(sym)
        if block is None:
            continue
        if text[pos : pos + 20].startswith("#ifndef SSX3_HOST"):
            continue
        depth = 1
        i = pos + len("#ifdef SKIP_ASM")
        while depth > 0:
            m2 = re.search(r"#(ifdef|ifndef|if|endif)\b", text[i:])
            if not m2:
                break
            tag = m2.group(1)
            i += m2.end()
            if tag in ("ifdef", "ifndef", "if"):
                depth += 1
            elif tag == "endif":
                depth -= 1
        body = text[pos : i - len("#endif")]
        out.append(block)
        out.append(body)
        out.append("#endif\n")
        out.append("#endif\n")
        pos = i
        changed += 1
    if changed:
        path.write_text("".join(out), encoding="utf-8")
    return changed


def main() -> int:
    syms = set(sys.argv[1:]) if len(sys.argv) > 1 else set()
    if not syms:
        print("usage: wrap_ps2_retail_skip_asm.py SYM [...]", file=sys.stderr)
        return 1
    total = 0
    for cpp in sorted(OBJECT.glob("*.cpp")):
        n = wrap_file(cpp, syms)
        if n:
            print(f"{cpp.name}: wrapped {n}")
            total += n
    print(f"[wrap] {total} function(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
