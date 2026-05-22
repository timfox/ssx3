#!/usr/bin/env python3
"""Extract SSX3 title-screen textures from disc SHPS archives into assets/host/menu/."""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
from ensure_venv_deps import ensure_venv_deps  # noqa: E402
OUT = ROOT / "assets" / "host" / "menu"
EAGM = Path("/tmp/EA-Graphics-Manager")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--disc",
        type=Path,
        default=ROOT / "disc",
        help="Path to extracted SSX3 disc root (default: ./disc)",
    )
    args = parser.parse_args()

    ensure_venv_deps("reversebox", "Pillow")

    if not (EAGM / "src").is_dir():
        print(f"[menu]    cloning EA-Graphics-Manager → {EAGM}")
        subprocess.run(
            [
                "git",
                "clone",
                "--depth",
                "1",
                "https://github.com/bartlomiejduda/EA-Graphics-Manager.git",
                str(EAGM),
            ],
            check=True,
        )

    sys.path.insert(0, str(EAGM))
    from src.EA_Image import ea_image_main  # noqa: E402
    from src.EA_Image.common_ea_dir import (  # noqa: E402
        get_palette_info_dto_from_dir_entry,
        handle_image_swizzle_logic,
        is_image_compressed,
        is_image_swizzled,
    )
    from src.EA_Image.ea_image_decoder import decode_image_data_by_entry_type  # noqa: E402
    from PIL import Image  # noqa: E402
    from reversebox.compression.compression_refpack import RefpackHandler  # noqa: E402

    def load_ssh(path: Path):
        ea = ea_image_main.EAImage()
        with path.open("rb") as f:
            status = ea.check_file_signature_and_size(f)
            if status[0] != "OK":
                raise RuntimeError(status)
            f.seek(0)
            ea.parse_header(f, str(path), path.name)
            ea.parse_directory(f)
            ea.parse_bin_attachments(f)
        return ea

    def decode_entry(ea, entry) -> Image.Image:
        entry_type = entry.h_record_id & 0x7F
        image_data = entry.raw_data
        if is_image_compressed(entry.h_record_id):
            image_data = RefpackHandler().decompress_data(image_data)
        if is_image_swizzled(entry):
            image_data = handle_image_swizzle_logic(
                image_data, entry_type, entry.h_width, entry.h_height, ea.sign, False
            )
        pal = get_palette_info_dto_from_dir_entry(entry, ea)
        rgba = decode_image_data_by_entry_type(entry_type, image_data, pal, entry)
        if not rgba:
            raise RuntimeError(f"decode failed for {entry.tag!r}")
        return Image.frombytes("RGBA", (entry.h_width, entry.h_height), rgba)

    def save(img: Image.Image, name: str) -> None:
        OUT.mkdir(parents=True, exist_ok=True)
        out_path = OUT / name
        img.save(out_path)
        print("wrote", out_path, img.size)

    disc = args.disc
    ui = load_ssh(disc / "data/ui/fe_1.ssh")

    ssxt = None
    moun = None
    for entry in ui.dir_entry_list:
        tag = entry.tag.strip()
        img = decode_entry(ui, entry)
        if tag == "ssxt":
            ssxt = img
        elif tag == "moun":
            moun = img
        elif tag == "Widg":
            save(img, "menu_panel.png")
        elif tag == "hudp":
            save(img, "ui_hudp.png")
        elif tag == "over":
            save(img, "ui_over.png")

    if ssxt is None:
        print("error: ssxt entry not found in data/ui/fe_1.ssh")
        return 1

    # Title: icy logo band from ssxt (exclude rider silhouettes below).
    w, h = ssxt.size
    logo_crop = ssxt.crop((0, 0, w, int(h * 0.54)))
    save(logo_crop, "title_logo.png")
    save(ssxt, "ssx3_logo.png")

    if moun is not None:
        save(moun, "title_mountain.png")

    print("\nTitle screen assets ready. Run: out/ssx3-native --gfx --no-boot-videos")
    print("hint: python3 scripts/extract_menu_font.py --disc disc  (retail FE font)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
