#!/usr/bin/env python3
"""Copy FE layout binaries (.lui) and related config from disc."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "host" / "ui" / "layouts"

LUI_FILES = [
    "data/ui/fe.lui",
    "data/ui/fl.lui",
    "data/ui/gl.lui",
    "data/ui/ov.lui",
    "data/ui/su.lui",
]

CONFIG_FILES = [
    "data/config/input2.map",
    "data/config/input.map",
]


def file_digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()[:16]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    args = parser.parse_args()

    disc = args.disc.resolve()
    OUT.mkdir(parents=True, exist_ok=True)
    config_out = ROOT / "assets" / "host" / "config"
    config_out.mkdir(parents=True, exist_ok=True)

    manifest: dict[str, object] = {"layouts": [], "config": []}

    for rel in LUI_FILES:
        src = disc / rel
        if not src.is_file():
            print(f"[lui]     skip missing {rel}")
            continue
        dst = OUT / src.name
        shutil.copy2(src, dst)
        manifest["layouts"].append(
            {
                "source": rel,
                "file": str(dst.relative_to(ROOT)),
                "size": src.stat().st_size,
                "sha256_16": file_digest(src),
                "magic": src.open("rb").read(4).decode("ascii", errors="replace"),
            }
        )
        print(f"[lui]     {rel} → {dst.relative_to(ROOT)}")

    for rel in CONFIG_FILES:
        src = disc / rel
        if not src.is_file():
            continue
        dst = config_out / src.name
        shutil.copy2(src, dst)
        manifest["config"].append(
            {
                "source": rel,
                "file": str(dst.relative_to(ROOT)),
                "size": src.stat().st_size,
            }
        )
        print(f"[lui]     {rel} → {dst.relative_to(ROOT)}")

    manifest_path = OUT / "layouts_manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"[lui]     manifest → {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
