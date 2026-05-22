"""EA BIGF archive helpers (shared by extract scripts)."""

from __future__ import annotations

import struct
from pathlib import Path


def read_be32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def list_big_entries(data: bytes) -> list[tuple[str, int, int]]:
    if data[:4] != b"BIGF":
        raise ValueError("not a BIGF archive")
    nfiles = read_be32(data, 8)
    cursor = 16
    entries: list[tuple[str, int, int]] = []
    for _ in range(nfiles):
        offset = read_be32(data, cursor)
        size = read_be32(data, cursor + 4)
        cursor += 8
        end = data.index(0, cursor)
        name = data[cursor:end].decode("latin-1", errors="replace")
        cursor = end + 1
        entries.append((name, offset, size))
    return entries


def extract_big_entry(data: bytes, entry_name: str) -> bytes | None:
    want = entry_name.replace("/", "\\").lower()
    for name, offset, size in list_big_entries(data):
        if name.lower() == want or name.lower().endswith(want.split("\\")[-1]):
            return data[offset : offset + size]
    return None


def read_big_file(path: Path, entry_name: str) -> bytes | None:
    return extract_big_entry(path.read_bytes(), entry_name)
