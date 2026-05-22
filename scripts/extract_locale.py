#!/usr/bin/env python3
"""Extract SSX3 locale (.loc) strings from disc into assets/host/locale/."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from lib.locale_extract import build_string_map, extract_loc_file  # noqa: E402

OUT = ROOT / "assets" / "host" / "locale"
DEFAULT_LOC_DIR = "data/locale"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument(
        "--locale-dir",
        type=Path,
        default=None,
        help="Override locale directory (default: disc/data/locale)",
    )
    args = parser.parse_args()

    loc_dir = args.locale_dir or (args.disc.resolve() / DEFAULT_LOC_DIR)
    if not loc_dir.is_dir():
        print(f"[locale]  missing {loc_dir}")
        return 1

    OUT.mkdir(parents=True, exist_ok=True)
    manifest: dict[str, object] = {"locale_dir": str(loc_dir), "files": {}}

    for path in sorted(loc_dir.glob("*.loc")):
        payload = extract_loc_file(path)
        out_path = OUT / f"{path.stem}.json"
        out_path.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")
        manifest["files"][path.name] = {
            "path": str(out_path.relative_to(ROOT)),
            "string_count": len(payload["strings"]),
            "size": payload["size"],
        }
        print(f"[locale]  {path.name} → {out_path.name} ({len(payload['strings'])} strings)")

    string_map, warnings = build_string_map(loc_dir)
    for line in warnings:
        print(f"[locale]  curated map warning: {line}", file=sys.stderr)

    map_path = OUT / "fe_strings.json"
    map_path.write_text(json.dumps(string_map, ensure_ascii=False, indent=2), encoding="utf-8")
    manifest["fe_strings"] = str(map_path.relative_to(ROOT))
    print(f"[locale]  fe_strings.json ({len(string_map)} curated keys)")

    manifest_path = OUT / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"[locale]  manifest → {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
