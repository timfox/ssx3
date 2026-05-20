#!/usr/bin/env python3
"""Patch EE-GCC EABI64 objects to O32 e_flags so they link with gnu-as output."""

import struct
import sys

# EF_MIPS_ABI: O32 from mips-linux-gnu-as -mabi=32 objects
O32_E_FLAGS = 0x20921101


def patch(path: str) -> None:
    with open(path, "r+b") as f:
        data = bytearray(f.read())
        if len(data) < 0x28:
            raise ValueError(f"{path}: not a valid ELF32 file")
        struct.pack_into("<I", data, 0x24, O32_E_FLAGS)
        f.seek(0)
        f.write(data)


def main() -> int:
    for path in sys.argv[1:]:
        patch(path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
