#!/usr/bin/env python3
"""Extract retail .word list from splat asm comments and emit *_ps2.s."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def words_from_splat(asm_path: Path, glabel: str, max_words: int | None = None) -> list[int]:
    text = asm_path.read_text()
    in_func = False
    words: list[int] = []
    for line in text.splitlines():
        if f"glabel {glabel}" in line:
            in_func = True
            continue
        if in_func and "glabel " in line and glabel not in line:
            break
        m = re.search(r"/\*\s+[0-9A-F]+\s+[0-9A-F]+\s+([0-9A-F]{8})\s+\*/", line)
        if in_func and m:
            h = m.group(1)
            w = int(h[6:8] + h[4:6] + h[2:4] + h[0:2], 16)
            words.append(w)
            if max_words and len(words) >= max_words:
                break
    return words


def emit_ps2(out_path: Path, symbol: str, words: list[int]) -> None:
    lines = [
        ".include \"macro.inc\"",
        "",
        ".set noat",
        ".set noreorder",
        "",
        ".section .text, \"ax\"",
        "",
        f".global {symbol}",
        f".ent {symbol}",
        f"{symbol}:",
    ]
    for w in words:
        lines.append(f"    .word  0x{w:08x}")
    lines.append(f".end {symbol}")
    lines.append("")
    out_path.write_text("\n".join(lines))


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("asm")
    ap.add_argument("glabel")
    ap.add_argument("out")
    ap.add_argument("--words", type=int, default=None)
    args = ap.parse_args()
    words = words_from_splat(Path(args.asm), args.glabel, args.words)
    if not words:
        raise SystemExit(f"no words for {args.glabel}")
    symbol = args.glabel
    emit_ps2(Path(args.out), symbol, words)
    print(f"wrote {args.out} ({len(words)} words, {len(words) * 4} bytes)")


if __name__ == "__main__":
    main()
