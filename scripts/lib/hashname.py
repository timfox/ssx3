"""EA tHashName32 (matches src/hashvalue.cpp SKIP_ASM)."""

from __future__ import annotations


def hash32(name: str) -> int:
    h = 0
    for c in name.encode("latin-1"):
        h = (h << 4) + c
        h &= 0xFFFFFFFF
        top = h & 0xF0000000
        if top:
            h = ((h ^ (top >> 23)) ^ top) & 0xFFFFFFFF
    return h
