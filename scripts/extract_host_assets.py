#!/usr/bin/env python3
"""Run all host asset extractors against an SSX3 disc rip."""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

STEPS = [
    ("locale", "extract_locale.py"),
    ("font", "extract_menu_font.py"),
    ("ui", "extract_ui_archives.py"),
    ("layouts", "extract_ui_layouts.py"),
    ("lui_probe", "parse_lui.py"),
    ("layout", "export_fe_layout.py"),
    ("ui_big", "extract_ui_big.py"),
    ("elf_fe", "extract_elf_fe_strings.py"),
    ("menu_png", "extract_menu_assets.py"),
    ("audio", "extract_audio_big.py"),
    ("music_inv", "extract_music_inventory.py"),
    ("manifest", "extract_disc_manifest.py"),
]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--force", action="store_true", help="Pass --force to audio extract")
    parser.add_argument("--skip", nargs="*", choices=[s[0] for s in STEPS])
    parser.add_argument("--dump-audio-raw", action="store_true")
    parser.add_argument(
        "--flat-portraits-only",
        action="store_true",
        help="Pass --flat-only to extract_ui_big.py (skip small portraits)",
    )
    args = parser.parse_args()

    disc = args.disc.resolve()
    if not (disc / "SYSTEM.CNF").is_file():
        print(f"[extract] missing disc at {disc} (need SYSTEM.CNF)")
        return 1

    skip = set(args.skip or [])
    py = sys.executable
    for name, script in STEPS:
        if name in skip:
            print(f"[extract] skip {name}")
            continue
        cmd = [py, str(ROOT / "scripts" / script), "--disc", str(disc)]
        if name == "audio":
            if args.force:
                cmd.append("--force")
            if args.dump_audio_raw:
                cmd.append("--dump-raw")
        if name == "ui_big" and args.flat_portraits_only:
            cmd.append("--flat-only")
        print(f"\n[extract] === {name} ===")
        subprocess.run(cmd, check=True)

    print("\n[extract] done → assets/host/")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
