#!/usr/bin/env python3
"""Generate config/undefined_syms_auto.txt from unresolved D_* symbols."""

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OBJ_DIR = ROOT / "obj"
OUT = ROOT / "config" / "undefined_syms_auto.txt"


def run_nm(args: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["mips-linux-gnu-nm", *args],
        capture_output=True,
        text=True,
        check=False,
    )


def parse_undefined(stdout: str) -> set[str]:
    symbols: set[str] = set()
    for line in stdout.splitlines():
        parts = line.split()
        if len(parts) >= 2:
            symbols.add(parts[-1])
    return symbols


def parse_defined(stdout: str) -> set[str]:
    symbols: set[str] = set()
    for line in stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3:
            symbols.add(parts[-1])
    return symbols


def symbol_value(symbol: str) -> int | None:
    try:
        return int(symbol[2:], 16)
    except ValueError:
        return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", nargs="?", type=Path, default=OUT)
    args = parser.parse_args()

    if not OBJ_DIR.is_dir():
        print("No obj/ directory; build objects first.", file=sys.stderr)
        return 1

    obj_paths = sorted(OBJ_DIR.rglob("*.o"))
    if not obj_paths:
        print("No object files found; build objects first.", file=sys.stderr)
        return 1

    undef: set[str] = set()
    defined: set[str] = set()
    for obj_path in obj_paths:
        undef_result = run_nm(["-u", str(obj_path)])
        if undef_result.returncode != 0:
            print(undef_result.stderr, end="", file=sys.stderr)
            return undef_result.returncode
        undef.update(parse_undefined(undef_result.stdout))

        defined_result = run_nm(["-g", "--defined-only", str(obj_path)])
        if defined_result.returncode != 0:
            print(defined_result.stderr, end="", file=sys.stderr)
            return defined_result.returncode
        defined.update(parse_defined(defined_result.stdout))

    d_syms = sorted(
        s for s in (undef - defined)
        if s.startswith("D_") and symbol_value(s) is not None
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", encoding="utf-8") as f:
        for sym in d_syms:
            f.write(f"{sym} = 0x{symbol_value(sym):08X};\n")

    print(f"Wrote {len(d_syms)} symbols to {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
