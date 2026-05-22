#!/usr/bin/env python3
"""Decode SSX3 front-end banks from audio.big into assets/host/audio/*.wav."""

from __future__ import annotations

import argparse
import shutil
import struct
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "host" / "audio"
VGM_DIR = ROOT / "tools" / "vgmstream"
VGM_CLI = VGM_DIR / "vgmstream-cli"
VGM_URL = (
    "https://github.com/vgmstream/vgmstream-releases/releases/download/"
    "nightly/vgmstream-linux-cli.tar.gz"
)

# vgmstream subsong indices are 1-based.
EXTRACTS = [
    ("data/audio/audio.big", "data\\audio\\BC_AmbLoop1.bnk", 1, "title_ambient.wav"),
    # Subsong 1 is the ~2.3s stereo menu loop; 4–6 are short stems (one-shots in-game).
    ("data/audio/audio.big", "data\\audio\\SSX3Menu.bnk", 1, "menu_music.wav"),
    ("data/audio/audio.big", "data\\audio\\SSX3Menu.bnk", 4, "menu_stem_a.wav"),
    ("data/audio/audio.big", "data\\audio\\SSX3Menu.bnk", 5, "menu_stem_b.wav"),
    ("data/audio/audio.big", "data\\audio\\SSX3Menu.bnk", 6, "menu_stem_c.wav"),
    ("data/audio/audio.big", "data\\audio\\FE Menu.bnk", 2, "sfx_navigate.wav"),
    ("data/audio/audio.big", "data\\audio\\FE Menu.bnk", 1, "sfx_confirm.wav"),
    ("data/audio/audio.big", "data\\audio\\FE Menu.bnk", 5, "sfx_back.wav"),
    ("data/audio/audio.big", "data\\audio\\FE Menu.bnk", 4, "sfx_start.wav"),
]


def read_be32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def extract_big_entry(disc: Path, big_rel: str, entry_name: str) -> bytes | None:
    big_path = disc / big_rel.replace("\\", "/")
    if not big_path.is_file():
        return None
    data = big_path.read_bytes()
    if data[:4] != b"BIGF":
        return None
    nfiles = read_be32(data, 8)
    cursor = 16
    want = entry_name.replace("/", "\\").lower()
    for _ in range(nfiles):
        offset = read_be32(data, cursor)
        size = read_be32(data, cursor + 4)
        cursor += 8
        end = data.index(0, cursor)
        name = data[cursor:end].decode("latin-1", errors="replace")
        cursor = end + 1
        if name.lower() == want or name.lower().endswith(want.split("\\")[-1]):
            return data[offset : offset + size]
    return None


def ensure_vgmstream() -> Path:
    if VGM_CLI.is_file():
        return VGM_CLI
    VGM_DIR.mkdir(parents=True, exist_ok=True)
    archive = VGM_DIR / "vgmstream-linux-cli.tar.gz"
    print(f"[audio]   downloading vgmstream → {archive}")
    urllib.request.urlretrieve(VGM_URL, archive)
    with tarfile.open(archive, "r:gz") as tar:
        tar.extractall(VGM_DIR)
    if not VGM_CLI.is_file():
        raise RuntimeError("vgmstream-cli missing after extract")
    VGM_CLI.chmod(0o755)
    return VGM_CLI


def decode_subsong(vgm: Path, bank_bytes: bytes, subsong: int, out_wav: Path) -> bool:
    tmp = OUT / "_tmp.bnk"
    tmp.write_bytes(bank_bytes)
    prefix = out_wav.with_suffix("")
    if prefix.exists():
        shutil.rmtree(prefix) if prefix.is_dir() else prefix.unlink()
    cmd = [str(vgm), "-s", str(subsong), "-o", str(prefix), str(tmp)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    tmp.unlink(missing_ok=True)
    if result.returncode != 0:
        print(result.stderr or result.stdout, file=sys.stderr)
        return False
    # vgmstream may omit .wav in the output path.
    produced = None
    for candidate in [prefix.with_suffix(".wav"), prefix, OUT / "_tmp.wav"]:
        if candidate.is_file():
            produced = candidate
            break
    if produced is None:
        for path in OUT.glob(prefix.name + "*"):
            if path.is_file():
                produced = path
                break
    if produced is None:
        return False
    out_wav.parent.mkdir(parents=True, exist_ok=True)
    if produced != out_wav:
        shutil.move(str(produced), str(out_wav))
    return out_wav.is_file() and out_wav.stat().st_size > 44


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--disc",
        type=Path,
        default=ROOT / "disc",
        help="Path to extracted SSX3 disc root (default: ./disc)",
    )
    parser.add_argument("--force", action="store_true", help="Re-decode even if outputs exist")
    args = parser.parse_args()

    disc = args.disc.resolve()
    audio_big = disc / "data/audio/audio.big"
    if not audio_big.is_file():
        print(f"[audio]   missing {audio_big}")
        return 1

    try:
        vgm = ensure_vgmstream()
    except OSError as exc:
        print(f"[audio]   vgmstream setup failed: {exc}")
        return 1

    OUT.mkdir(parents=True, exist_ok=True)
    ok = True
    for big_rel, entry, subsong, out_name in EXTRACTS:
        out_path = OUT / out_name
        if out_path.is_file() and not args.force:
            print(f"[audio]   skip {out_name} (exists)")
            continue
        blob = extract_big_entry(disc, big_rel, entry)
        if blob is None:
            print(f"[audio]   missing {entry} in {big_rel}")
            ok = False
            continue
        print(f"[audio]   {out_name} ← {entry} #{subsong}")
        if not decode_subsong(vgm, blob, subsong, out_path):
            print(f"[audio]   decode failed: {out_name}")
            ok = False

    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
