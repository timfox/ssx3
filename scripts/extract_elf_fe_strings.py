#!/usr/bin/env python3
"""Index FE-related strings and hash32 names from the built ELF."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from lib.hashname import hash32  # noqa: E402

OUT = ROOT / "assets" / "host" / "decomp"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--elf", type=Path, default=ROOT / "out" / "SLUS_207.72")
    args = parser.parse_args()

    if not args.elf.is_file():
        print(f"[elf]     missing {args.elf} (build with ninja first)")
        return 1

    proc = subprocess.run(
        ["strings", "-n", "4", str(args.elf)],
        capture_output=True,
        text=True,
        check=True,
    )
    lines = [ln.strip() for ln in proc.stdout.splitlines() if ln.strip()]

    fe_states = sorted({s for s in lines if s.startswith("cFEState")})
    fe_paths = sorted({s for s in lines if s.startswith("data/") and ("ui" in s or "locale" in s or "audio" in s)})
    menu_symbols = sorted(
        {
            s
            for s in lines
            if "Menu" in s
            or s.startswith("kT_")
            or s.startswith("ConfirmMenu")
            or s.startswith("MenuRoom")
        }
    )
    hash_index = {s: f"{hash32(s):08x}" for s in sorted(set(fe_states + menu_symbols))}

    payload = {
        "elf": str(args.elf),
        "fe_state_count": len(fe_states),
        "fe_states": fe_states,
        "disc_paths": fe_paths,
        "menu_symbols": menu_symbols,
        "hash32_index": hash_index,
    }

    OUT.mkdir(parents=True, exist_ok=True)
    out_path = OUT / "fe_symbols.json"
    out_path.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    print(f"[elf]     {len(fe_states)} FE states, {len(menu_symbols)} menu symbols → {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
