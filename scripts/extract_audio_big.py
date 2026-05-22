#!/usr/bin/env python3
"""Inventory audio.big and decode front-end banks (wraps extract_menu_audio)."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from lib.bigf import extract_big_entry, list_big_entries  # noqa: E402

OUT = ROOT / "assets" / "host" / "audio"
RAW = OUT / "raw"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--force", action="store_true")
    parser.add_argument(
        "--dump-raw",
        action="store_true",
        help="Copy every .bnk from audio.big into assets/host/audio/raw/",
    )
    parser.add_argument("--skip-decode", action="store_true", help="Only write inventory/raw")
    args = parser.parse_args()

    disc = args.disc.resolve()
    big_path = disc / "data/audio/audio.big"
    if not big_path.is_file():
        print(f"[audio]   missing {big_path}")
        return 1

    data = big_path.read_bytes()
    entries = list_big_entries(data)
    OUT.mkdir(parents=True, exist_ok=True)

    inventory = [
        {"name": name, "offset": offset, "size": size}
        for name, offset, size in entries
    ]
    inv_path = OUT / "audio_big_inventory.json"
    inv_path.write_text(json.dumps(inventory, indent=2), encoding="utf-8")
    print(f"[audio]   audio.big: {len(entries)} entries → {inv_path.name}")

    if args.dump_raw:
        RAW.mkdir(parents=True, exist_ok=True)
        count = 0
        for name, _offset, _size in entries:
            if not name.lower().endswith(".bnk"):
                continue
            blob = extract_big_entry(data, name)
            if not blob:
                continue
            safe = name.replace("\\", "_").replace("/", "_")
            (RAW / safe).write_bytes(blob)
            count += 1
        print(f"[audio]   dumped {count} banks → {RAW.relative_to(ROOT)}")

    if not args.skip_decode:
        cmd = [
            sys.executable,
            str(ROOT / "scripts/extract_menu_audio.py"),
            "--disc",
            str(disc),
        ]
        if args.force:
            cmd.append("--force")
        subprocess.run(cmd, check=True)

    manifest = {
        "audio_big": "data/audio/audio.big",
        "entry_count": len(entries),
        "inventory": str(inv_path.relative_to(ROOT)),
    }
    (OUT / "manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
