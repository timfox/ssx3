#!/usr/bin/env python3
"""Extract SSX3 front-end textures from disc SHPS archives into assets/host/menu/."""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "host" / "menu"
EAGM = Path("/tmp/EA-Graphics-Manager/src")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--disc",
        type=Path,
        default=ROOT / "disc",
        help="Path to extracted SSX3 disc root (default: ./disc)",
    )
    args = parser.parse_args()

    if not EAGM.is_dir():
        print("Clone EA-Graphics-Manager to /tmp/EA-Graphics-Manager and pip install reversebox Pillow")
        return 1

    sys.path.insert(0, str(EAGM))
    from EA_Image import ea_image_main  # noqa: E402
    from EA_Image.common_ea_dir import (  # noqa: E402
        get_palette_info_dto_from_dir_entry,
        handle_image_swizzle_logic,
        is_image_compressed,
        is_image_swizzled,
    )
    from EA_Image.ea_image_decoder import decode_image_data_by_entry_type  # noqa: E402
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

    def export_entry(ea, entry, out_name: str) -> None:
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
            raise RuntimeError(f"decode failed for {out_name}")
        OUT.mkdir(parents=True, exist_ok=True)
        img = Image.frombytes("RGBA", (entry.h_width, entry.h_height), rgba)
        out_path = OUT / out_name
        img.save(out_path)
        print("wrote", out_path)

    disc = args.disc
    export_entry(load_ssh(disc / "data/fonts/splash.ssh"), load_ssh(disc / "data/fonts/splash.ssh").dir_entry_list[0], "ssx3_logo.png")
    fe = load_ssh(disc / "data/textures/fe_1.ssh")
    for entry in fe.dir_entry_list:
        export_entry(fe, entry, f"fe_{entry.tag.strip()}.png")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
