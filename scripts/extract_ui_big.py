#!/usr/bin/env python3
"""Extract SSH textures from UI BIG archives (charpic, mapgfx, courspic)."""

from __future__ import annotations

import argparse
import json
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from ensure_venv_deps import ensure_venv_deps  # noqa: E402
from lib.bigf import extract_big_entry, list_big_entries  # noqa: E402
from lib.ea_ssh import decode_entry, load_ssh, load_ssh_decoder  # noqa: E402

ARCHIVES = [
    ("data/ui/charpic.big", "characters"),
    ("data/ui/mapgfx.big", "map"),
    ("data/ui/courspic.big", "course"),
]

OUT = ROOT / "assets" / "host"


def extract_ssh_blob(blob: bytes, entry_name: str, out_dir: Path, helpers) -> list[dict[str, object]]:
    written: list[dict[str, object]] = []
    with tempfile.NamedTemporaryFile(suffix=".ssh", delete=False) as tmp:
        tmp.write(blob)
        tmp.flush()
        path = Path(tmp.name)
    try:
        ea = load_ssh(path)
        for entry in ea.dir_entry_list:
            tag = entry.tag.strip() or "untagged"
            try:
                img = decode_entry(ea, entry, helpers)
            except Exception as exc:
                print(f"[big]     {entry_name} {tag!r}: {exc}")
                continue
            safe = entry_name.replace(".ssh", "").replace("/", "_")
            out_name = f"{safe}_{tag}.png"
            out_path = out_dir / out_name
            img.save(out_path)
            written.append(
                {
                    "entry": entry_name,
                    "tag": tag,
                    "file": str(out_path.relative_to(ROOT)),
                    "size": list(img.size),
                }
            )
            print(f"[big]     {entry_name} → {out_path.relative_to(ROOT)}")
    finally:
        path.unlink(missing_ok=True)
    return written


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--only", choices=["characters", "map", "course"], nargs="*")
    parser.add_argument("--flat-only", action="store_true", help="Skip *_small_flat.ssh entries")
    args = parser.parse_args()

    ensure_venv_deps("reversebox", "Pillow")
    helpers = load_ssh_decoder()[1]
    disc = args.disc.resolve()
    manifest: dict[str, object] = {"archives": {}}

    for big_rel, subdir in ARCHIVES:
        if args.only and subdir not in args.only:
            continue
        big_path = disc / big_rel
        if not big_path.is_file():
            print(f"[big]     missing {big_rel}")
            continue
        data = big_path.read_bytes()
        out_dir = OUT / subdir
        out_dir.mkdir(parents=True, exist_ok=True)
        entries_written: list[dict[str, object]] = []
        for name, _off, _size in list_big_entries(data):
            if not name.lower().endswith(".ssh"):
                continue
            if args.flat_only and "small" in name.lower():
                continue
            blob = extract_big_entry(data, name)
            if not blob:
                continue
            entries_written.extend(extract_ssh_blob(blob, name, out_dir, helpers))
        manifest["archives"][big_rel] = {
            "output_dir": str(out_dir.relative_to(ROOT)),
            "textures": entries_written,
        }
        inv_path = out_dir / "inventory.json"
        inv_path.write_text(
            json.dumps([{"name": n, "size": s} for n, _o, s in list_big_entries(data)], indent=2),
            encoding="utf-8",
        )

    manifest_path = OUT / "ui_big_manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"[big]     manifest → {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
