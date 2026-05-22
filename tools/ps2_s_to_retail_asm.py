#!/usr/bin/env python3
"""Turn src/mem/*_ps2.s stubs into PS2_RETAIL_ASM insn blocks (raw .word literals)."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

WORD_RE = re.compile(r"\.word\s+0x([0-9A-Fa-f]+)")
GLABEL_RE = re.compile(r"^\.global\s+(\w+)|^glabel\s+(\w+)")


def symbol_from_ps2(ps2_path: Path) -> str:
    text = ps2_path.read_text(encoding="utf-8", errors="replace")
    for line in text.splitlines():
        m = GLABEL_RE.match(line.strip())
        if m:
            return m.group(1) or m.group(2)
    raise ValueError(f"no symbol in {ps2_path}")


def words_from_ps2(ps2_path: Path) -> list[str]:
    text = ps2_path.read_text(encoding="utf-8", errors="replace")
    words = WORD_RE.findall(text)
    if not words:
        raise ValueError(f"{ps2_path}: no .word literals")
    return words


def format_word_block(words: list[str]) -> str:
    parts = [f'    ".word 0x{w}\\n\\t"\n' for w in words]
    return "".join(parts)


def fn_declaration(obj_text: str, glabel: str) -> str | None:
    for pat in (
        rf"(?:^|\n)(?:extern\s+\"C\"\s+)?([\w\s\*]+?)\s+{re.escape(glabel)}\s*\(([^)]*)\)\s*;",
        rf"(?:^|\n)(?:extern\s+\"C\"\s+)?([\w\s\*]+?)\s+{re.escape(glabel)}\s*\(([^)]*)\)\s*\{{",
    ):
        m = re.search(pat, obj_text, re.M)
        if m:
            ret, args = m.group(1).strip(), m.group(2).strip()
            if not re.match(r"^(?:void|int|unsigned|char|float|double|static\b)", ret):
                continue
            return f"{ret} {glabel}({args});"
    return None


def retail_asm_from_ps2(ps2_path: Path, obj_text: str | None = None, glabel: str | None = None) -> tuple[str, str, int]:
    """Return (symbol, PS2_RETAIL_ASM_ONLY body, arity hint)."""
    symbol = symbol_from_ps2(ps2_path)
    words = words_from_ps2(ps2_path)
    block = format_word_block(words)
    body = f"PS2_RETAIL_ASM_ONLY(\n    {symbol},\n{block})"
    arity = 0
    if obj_text and glabel:
        decl = fn_declaration(obj_text, glabel)
        if decl:
            inner = decl[decl.index("(") + 1 : decl.index(")")]
            arity = 0 if not inner.strip() or inner.strip() == "void" else inner.count(",") + 1
    return symbol, body, arity


def main() -> int:
    import sys

    if len(sys.argv) < 2:
        print("usage: ps2_s_to_retail_asm.py <ps2.s>...")
        return 1
    for arg in sys.argv[1:]:
        path = Path(arg)
        sym, body, arity = retail_asm_from_ps2(path)
        print(f"/* {path.name} arity={arity} */")
        print(body)
        print()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
