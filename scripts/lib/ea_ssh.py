"""Decode EA SHPS (.ssh) archives via EA-Graphics-Manager."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

EAGM = Path("/tmp/EA-Graphics-Manager")


def ensure_eagm() -> Path:
    if not (EAGM / "src").is_dir():
        print(f"[ssh]     cloning EA-Graphics-Manager → {EAGM}")
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
    return EAGM


def load_ssh_decoder():
    ensure_eagm()
    if str(EAGM) not in sys.path:
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

    return ea_image_main, (
        get_palette_info_dto_from_dir_entry,
        handle_image_swizzle_logic,
        is_image_compressed,
        is_image_swizzled,
        decode_image_data_by_entry_type,
        Image,
        RefpackHandler,
    )


def load_ssh(path: Path):
    ea_image_main, _ = load_ssh_decoder()
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


def decode_entry(ea, entry, helpers) -> object:
    (
        get_palette_info_dto_from_dir_entry,
        handle_image_swizzle_logic,
        is_image_compressed,
        is_image_swizzled,
        decode_image_data_by_entry_type,
        Image,
        RefpackHandler,
    ) = helpers

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
