#!/usr/bin/env python3
"""Bake a TTF/OTF into host menu font assets (host_font_atlas.png + host_font_metrics.bin).

Uses the same FEFN metrics container as scripts/extract_menu_font.py so the native
loader can swap between retail (fe_font_*) and custom (host_font_*) at runtime.
When host_font_* exists, retail fe_font_* is still loaded for <start> controller icons.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "host" / "menu"
METRICS_MAGIC = b"FEFN"
METRICS_VERSION = 1

# Printable ASCII + a few symbols used on FE screens.
CODEPOINTS = list(range(0x20, 0x7F)) + [0xA9, 0xAE, 0x2013, 0x2026]

DEFAULT_FONTS = [
    "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
    "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
    "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
]


def pick_font(explicit: Path | None) -> Path:
    if explicit is not None:
        if not explicit.is_file():
            raise FileNotFoundError(explicit)
        return explicit
    for candidate in DEFAULT_FONTS:
        path = Path(candidate)
        if path.is_file():
            return path
    raise FileNotFoundError(
        "no TTF found; pass --font /path/to/Font.ttf (DejaVu Sans Bold works well)"
    )


def bake(font_path: Path, size: int, atlas_w: int, atlas_h: int) -> tuple[int, int, int, int, list[dict]]:
    from PIL import Image, ImageDraw, ImageFont

    font = ImageFont.truetype(str(font_path), size=size)
    ascent, descent = font.getmetrics()

    glyphs: list[dict] = []
    cursor_x = 2
    cursor_y = 2
    row_h = 0

    for code in CODEPOINTS:
        ch = chr(code)
        bbox = font.getbbox(ch)
        if bbox is None:
            continue
        left, top, right, bottom = bbox
        gw = max(right - left, 1)
        gh = max(bottom - top, 1)
        advance = max(int(font.getlength(ch)), gw)

        if cursor_x + gw + 2 > atlas_w:
            cursor_x = 2
            cursor_y += row_h + 2
            row_h = 0
        if cursor_y + gh + 2 > atlas_h:
            raise RuntimeError(f"atlas {atlas_w}x{atlas_h} too small for {len(CODEPOINTS)} glyphs")

        x_off = -left
        y_off = -top + ascent
        glyphs.append(
            {
                "code": code,
                "width": gw,
                "height": gh,
                "u": cursor_x,
                "v": cursor_y,
                "advance": min(advance, 255),
                "x_offset": max(-128, min(127, x_off)),
                "y_offset": max(-128, min(127, y_off - ascent)),
            }
        )
        cursor_x += gw + 2
        row_h = max(row_h, gh)

    atlas = Image.new("RGBA", (atlas_w, atlas_h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(atlas)
    for glyph in glyphs:
        ch = chr(glyph["code"])
        draw.text(
            (glyph["u"] - glyph["x_offset"], glyph["v"] - glyph["y_offset"] + ascent),
            ch,
            font=font,
            fill=(255, 255, 255, 255),
        )

    OUT.mkdir(parents=True, exist_ok=True)
    png_path = OUT / "host_font_atlas.png"
    atlas.save(png_path)
    print(f"[font]    {png_path.name} {atlas_w}x{atlas_h} from {font_path.name} @{size}px")

    return ascent, descent, atlas_w, atlas_h, glyphs


def write_metrics_bin(ascent: int, descent: int, atlas_w: int, atlas_h: int, glyphs: list[dict]) -> Path:
    out_path = OUT / "host_font_metrics.bin"
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
    parser.add_argument("--font", type=Path, default=None, help="TTF/OTF path")
    parser.add_argument("--size", type=int, default=22, help="Raster size in px")
    parser.add_argument("--atlas", type=int, nargs=2, default=(512, 256), metavar=("W", "H"))
    args = parser.parse_args()

    try:
        from PIL import Image  # noqa: F401
    except ImportError:
        print("[font]    install Pillow: apt install python3-pil")
        return 1

    try:
        font_path = pick_font(args.font)
        ascent, descent, aw, ah, glyphs = bake(font_path, args.size, args.atlas[0], args.atlas[1])
        write_metrics_bin(ascent, descent, aw, ah, glyphs)
    except Exception as exc:  # noqa: BLE001
        print(f"[font]    bake failed: {exc}")
        return 1

    print("[font]    host_font_* ready — run with retail icons still extracted:")
    print("[font]    python3 scripts/extract_menu_font.py --disc disc")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
