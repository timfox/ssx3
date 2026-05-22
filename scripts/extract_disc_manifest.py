#!/usr/bin/env python3
"""Walk disc/data and write a manifest of FE-relevant assets."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "host" / "disc_manifest.json"

# Prefixes the native port cares about first.
DATA_PREFIXES = (
    "ui/",
    "textures/",
    "fonts/",
    "locale/",
    "audio/",
    "config/",
    "movies/",
)


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()[:16]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--full", action="store_true", help="Include all of disc/data/")
    args = parser.parse_args()

    data_root = args.disc.resolve() / "data"
    if not data_root.is_dir():
        print(f"[manifest] missing {data_root}")
        return 1

    files: list[dict[str, object]] = []
    for path in sorted(data_root.rglob("*")):
        if not path.is_file():
            continue
        rel = path.relative_to(data_root).as_posix()
        if not args.full and not any(rel.startswith(prefix) for prefix in DATA_PREFIXES):
            continue
        files.append(
            {
                "path": rel,
                "size": path.stat().st_size,
                "sha256_16": digest(path),
            }
        )

    payload = {
        "disc": str(args.disc.resolve()),
        "file_count": len(files),
        "files": files,
    }
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    print(f"[manifest] {len(files)} files → {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
