#!/usr/bin/env python3
"""Extract SSX3 front-end font (data/fonts/fefont.sfn) for native menu text."""

from __future__ import annotations

import argparse
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
from ensure_venv_deps import ensure_venv_deps  # noqa: E402

OUT = ROOT / "assets" / "host" / "menu"
EA_FONT = Path("/tmp/EA-Font-Manager")
FONT_SRC = "data/fonts/fefont.sfn"
METRICS_MAGIC = b"FEFN"
METRICS_VERSION = 1


def ensure_ea_font_manager() -> Path:
    if (EA_FONT / "src" / "EA_Font" / "ea_font_file.py").is_file():
        return EA_FONT
    EA_FONT.mkdir(parents=True, exist_ok=True)
    print(f"[font]    cloning EA-Font-Manager → {EA_FONT}")
    subprocess.run(
        ["git", "clone", "--depth", "1", "https://github.com/bartlomiejduda/EA-Font-Manager.git", str(EA_FONT)],
        check=True,
    )
    return EA_FONT


def signed_byte(value: int) -> int:
    return value - 256 if value > 127 else value


def parse_fefont(disc: Path) -> tuple[bytes, int, int, int, int, list[dict]]:
    sys.path.insert(0, str(ensure_ea_font_manager()))
    from reversebox.compression.compression_refpack import RefpackHandler  # noqa: E402
    from src.EA_Font.common_ea_dir import (  # noqa: E402
        get_palette_info_dto_from_dir_entry,
        handle_image_swizzle_logic,
        is_image_compressed,
        is_image_swizzled,
    )
    from src.EA_Font.ea_image_decoder import decode_image_data_by_entry_type  # noqa: E402
    from src.EA_Font.ea_font_file import EAFontFile  # noqa: E402
    from PIL import Image  # noqa: E402

    path = disc / FONT_SRC.replace("\\", "/")
    if not path.is_file():
        raise FileNotFoundError(path)

    ea = EAFontFile()
    with path.open("rb") as handle:
        ea.parse_file_header(handle, str(path), path.name)
        ea.parse_font_flags()
        ea.parse_character_table(handle)
        ea.parse_directory(handle)
        ea.parse_bin_attachments(handle)

    if not ea.dir_entry_list:
        raise RuntimeError("fefont.sfn has no atlas directory entry")

    entry = ea.dir_entry_list[0]
    entry_type = entry.h_record_id & 0x7F
    image_data = entry.raw_data
    if is_image_compressed(entry.h_record_id):
        image_data = RefpackHandler().decompress_data(image_data)
    if is_image_swizzled(entry):
        image_data = handle_image_swizzle_logic(
            image_data, entry_type, entry.h_width, entry.h_height, ea.fh_sign, False
        )
    palette = get_palette_info_dto_from_dir_entry(entry, ea)
    rgba = decode_image_data_by_entry_type(entry_type, image_data, palette, entry)
    if not rgba:
        raise RuntimeError("failed to decode fefont atlas")

    atlas_w = entry.h_width
    atlas_h = entry.h_height
    png_path = OUT / "fe_font_atlas.png"
    OUT.mkdir(parents=True, exist_ok=True)
    Image.frombytes("RGBA", (atlas_w, atlas_h), rgba).save(png_path)
    print(f"[font]    {png_path.name} {atlas_w}x{atlas_h}")

    glyphs: list[dict] = []
    for char in ea.character_entry_list:
        code = ord(char.index)
        if char.width == 0 or char.height == 0:
            continue
        glyphs.append(
            {
                "code": code,
                "width": char.width,
                "height": char.height,
                "u": char.u,
                "v": char.v,
                "advance": char.advance,
                "x_offset": signed_byte(char.x_offset),
                "y_offset": signed_byte(char.y_offset),
            }
        )

    glyphs.sort(key=lambda item: item["code"])
    return rgba, atlas_w, atlas_h, ea.fh_ascent, ea.fh_descent, glyphs


def write_metrics_bin(atlas_w: int, atlas_h: int, ascent: int, descent: int, glyphs: list[dict]) -> Path:
    out_path = OUT / "fe_font_metrics.bin"
    with out_path.open("wb") as handle:
        handle.write(METRICS_MAGIC)
        handle.write(struct.pack("<HII", METRICS_VERSION, atlas_w, atlas_h))
        handle.write(struct.pack("<BBH", ascent & 0xFF, descent & 0xFF, len(glyphs)))
        for glyph in glyphs:
            handle.write(
                struct.pack(
                    "<HBBHHBbb",
                    glyph["code"],
                    glyph["width"],
                    glyph["height"],
                    glyph["u"],
                    glyph["v"],
                    glyph["advance"],
                    glyph["x_offset"],
                    glyph["y_offset"],
                )
            )
    print(f"[font]    {out_path.name} ({len(glyphs)} glyphs)")
    return out_path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    args = parser.parse_args()

    try:
        ensure_venv_deps("reversebox", "Pillow")
        _, atlas_w, atlas_h, ascent, descent, glyphs = parse_fefont(args.disc.resolve())
        write_metrics_bin(atlas_w, atlas_h, ascent, descent, glyphs)
    except subprocess.CalledProcessError as exc:
        print(f"[font]    EA-Font-Manager clone failed: {exc}")
        return 1
    except Exception as exc:  # noqa: BLE001
        print(f"[font]    extract failed: {exc}")
        return 1

    print("[font]    retail FE font ready (fefont.sfn)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
