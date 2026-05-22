#!/usr/bin/env python3
"""List entries in music.big and speech.big (no decode)."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from lib.bigf import list_big_entries  # noqa: E402

OUT = ROOT / "assets" / "host" / "audio"
ARCHIVES = [
    "data/audio/music.big",
    "data/audio/speech.big",
]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    args = parser.parse_args()

    disc = args.disc.resolve()
    OUT.mkdir(parents=True, exist_ok=True)
    manifest: dict[str, object] = {}

    for rel in ARCHIVES:
        path = disc / rel
        if not path.is_file():
            print(f"[music]   skip missing {rel}")
            continue
        raw = path.read_bytes()
        if len(raw) < 16 or raw[:4] != b"BIGF":
            print(f"[music]   skip {rel} (empty or not BIGF, {len(raw)} bytes)")
            continue
        entries = [
            {"name": name, "offset": offset, "size": size}
            for name, offset, size in list_big_entries(raw)
        ]
        out_name = path.stem + "_inventory.json"
        out_path = OUT / out_name
        out_path.write_text(json.dumps(entries, indent=2), encoding="utf-8")
        manifest[rel] = {"entries": len(entries), "file": str(out_path.relative_to(ROOT))}
        print(f"[music]   {rel}: {len(entries)} entries → {out_path.name}")

    (OUT / "music_manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
