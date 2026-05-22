#!/usr/bin/env python3
"""Extract all FE-related SHPS archives from disc into assets/host/ui/."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from ensure_venv_deps import ensure_venv_deps  # noqa: E402
from lib.ea_ssh import decode_entry, load_ssh, load_ssh_decoder  # noqa: E402

OUT = ROOT / "assets" / "host" / "ui"

# Archives that feed the front-end (data/ui is authoritative; textures/ mirrors some).
SSH_SOURCES = [
    ("data/ui/fe_1.ssh", "fe_1"),
    ("data/ui/fl_1.ssh", "fl_1"),
    ("data/ui/gl_1.ssh", "gl_1"),
    ("data/ui/ov_1.ssh", "ov_1"),
    ("data/ui/su_1.ssh", "su_1"),
    ("data/fonts/splash.ssh", "splash"),
    ("data/fonts/menu.ssh", "menu_font"),
]

# Extra aliases for the title screen (same tags as extract_menu_assets.py).
TITLE_ALIASES = {
    ("fe_1", "ssxt"): "ssx3_logo.png",
    ("fe_1", "moun"): "title_mountain.png",
    ("fe_1", "Widg"): "menu_panel.png",
    ("fe_1", "hudp"): "ui_hudp.png",
    ("fe_1", "over"): "ui_over.png",
    ("fe_1", "PS2"): "ui_PS2.png",
}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--only", nargs="*", help="Limit to archive prefix (e.g. fe_1 ov_1)")
    args = parser.parse_args()

    ensure_venv_deps("reversebox", "Pillow")
    helpers = load_ssh_decoder()[1]
    disc = args.disc.resolve()
    OUT.mkdir(parents=True, exist_ok=True)

    manifest: dict[str, object] = {"archives": {}}
    menu_dir = ROOT / "assets" / "host" / "menu"
    menu_dir.mkdir(parents=True, exist_ok=True)

    for rel, prefix in SSH_SOURCES:
        if args.only and prefix not in args.only:
            continue
        path = disc / rel
        if not path.is_file():
            print(f"[ui]      skip missing {rel}")
            continue

        ea = load_ssh(path)
        archive_manifest: list[dict[str, object]] = []
        for entry in ea.dir_entry_list:
            tag = entry.tag.strip()
            if not tag:
                continue
            try:
                img = decode_entry(ea, entry, helpers)
            except Exception as exc:
                print(f"[ui]      {prefix}/{tag}: decode failed ({exc})")
                continue

            alias = TITLE_ALIASES.get((prefix, tag))
            if alias:
                out_name = alias
                out_path = menu_dir / out_name
            else:
                safe_tag = "".join(c if c.isalnum() or c in "-_" else "_" for c in tag)
                out_name = f"{prefix}_{safe_tag}.png"
                out_path = OUT / out_name

            img.save(out_path)
            archive_manifest.append(
                {
                    "tag": tag,
                    "file": str(out_path.relative_to(ROOT)),
                    "size": list(img.size),
                }
            )
            print(f"[ui]      {rel} {tag!r} → {out_path.relative_to(ROOT)}")

        if prefix == "fe_1":
            ssxt_path = menu_dir / "ssx3_logo.png"
            if ssxt_path.is_file():
                from PIL import Image

                ssxt = Image.open(ssxt_path)
                w, h = ssxt.size
                logo_crop = ssxt.crop((0, 0, w, int(h * 0.54)))
                title_path = menu_dir / "title_logo.png"
                logo_crop.save(title_path)
                archive_manifest.append(
                    {
                        "tag": "ssxt_crop",
                        "file": str(title_path.relative_to(ROOT)),
                        "size": list(logo_crop.size),
                    }
                )
                print(f"[ui]      title crop → {title_path.relative_to(ROOT)}")

        manifest["archives"][prefix] = {
            "source": rel,
            "entries": archive_manifest,
        }

    manifest_path = OUT / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"[ui]      manifest → {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
