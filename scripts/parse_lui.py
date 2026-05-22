#!/usr/bin/env python3
"""Probe EA IYF .lui files: header, P-widgets, Q-properties → JSON for analysis."""

from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from lib.hashname import hash32  # noqa: E402

OUT = ROOT / "assets" / "host" / "ui" / "lui_probe"
DEFAULT_LUI = [
    "data/ui/fe.lui",
    "data/ui/fl.lui",
    "data/ui/gl.lui",
    "data/ui/ov.lui",
    "data/ui/su.lui",
]


def parse_header(data: bytes) -> dict[str, object]:
    if data[:3] != b"IYF":
        raise ValueError("not IYF")
    version, a, sec_a, sec_b, sec_c, count = struct.unpack_from("<6I", data, 4)
    return {
        "magic": "IYF",
        "version": version,
        "section_offsets": [sec_a, sec_b, sec_c],
        "declared_count": count,
        "file_size": len(data),
    }


def parse_q_props(data: bytes, off: int, limit: int = 512) -> dict[int, int]:
    props: dict[int, int] = {}
    qoff = off + 16
    end = min(len(data), off + limit)
    while qoff < end - 8 and data[qoff : qoff + 2] == b"Q\x00":
        qid, tag, typ, _pad, val = struct.unpack_from("<5H", data, qoff + 2)
        if tag == 16 and typ == 2:
            props[qid] = val
        qoff += 16
    return props


def parse_p_widgets(data: bytes) -> list[dict[str, object]]:
    widgets: list[dict[str, object]] = []
    for m in re.finditer(b"P\x00\x08\x00", data):
        off = m.start()
        screen_hash = struct.unpack_from("<I", data, off + 8)[0]
        props = parse_q_props(data, off)
        if 6 not in props:
            continue
        widgets.append(
            {
                "offset": off,
                "screen_hash": screen_hash,
                "screen_hash_hex": f"{screen_hash:08x}",
                "x": props.get(6),
                "y": props.get(7),
                "w": props.get(0),
                "h": props.get(1),
                "props": {str(k): v for k, v in sorted(props.items())},
            }
        )
    return widgets


def q_property_stats(data: bytes) -> dict[str, object]:
    counts: dict[int, int] = {}
    samples: dict[int, list[int]] = {}
    off = 0
    while off < len(data) - 16:
        if data[off : off + 2] != b"Q\x00":
            off += 1
            continue
        qid, tag, typ, _pad, val = struct.unpack_from("<5H", data, off + 2)
        if tag == 16 and typ == 2:
            counts[qid] = counts.get(qid, 0) + 1
            if qid in (0, 1, 6, 7) and len(samples.get(qid, [])) < 12:
                samples.setdefault(qid, []).append(val)
        off += 16
    return {
        "q_record_count": data.count(b"Q\x00"),
        "by_property_id": {str(k): v for k, v in sorted(counts.items())},
        "samples": {str(k): v for k, v in samples.items()},
    }


def probe_lui(path: Path, name_hints: dict[int, str]) -> dict[str, object]:
    data = path.read_bytes()
    widgets = parse_p_widgets(data)
    for w in widgets:
        sh = w["screen_hash"]
        if isinstance(sh, int) and sh in name_hints:
            w["name_hint"] = name_hints[sh]
    return {
        "source": str(path),
        "header": parse_header(data),
        "p_widget_count": len(widgets),
        "p_widgets": widgets,
        "q_stats": q_property_stats(data),
        "marker_counts": {
            "P": data.count(b"P\x00\x08\x00"),
            "Q": data.count(b"Q\x00"),
            "L": data.count(b"L\x00\x00\x00"),
            "S": data.count(b"S\x00"),
            "W": data.count(b"W\x00"),
        },
    }


def load_elf_name_hints(elf_path: Path) -> dict[int, str]:
    if not elf_path.is_file():
        return {}
    import subprocess

    proc = subprocess.run(
        ["strings", "-n", "4", str(elf_path)],
        capture_output=True,
        text=True,
        check=False,
    )
    hints: dict[int, str] = {}
    for line in proc.stdout.splitlines():
        s = line.strip()
        if not s or len(s) > 64:
            continue
        if s.startswith("cFEState") or s.endswith("Menu") or s.startswith("Menu") or s.startswith("Confirm"):
            hints[hash32(s)] = s
    return hints


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=ROOT / "disc")
    parser.add_argument("--elf", type=Path, default=ROOT / "out" / "SLUS_207.72")
    parser.add_argument("--out", type=Path, default=OUT)
    args = parser.parse_args()

    hints = load_elf_name_hints(args.elf)
    args.out.mkdir(parents=True, exist_ok=True)

    manifest: dict[str, object] = {
        "files": {},
        "hash_hints": {f"{hv:08x}": name for hv, name in hints.items()},
    }

    for rel in DEFAULT_LUI:
        path = args.disc / rel
        if not path.is_file():
            print(f"[lui]     skip missing {rel}")
            continue
        payload = probe_lui(path, hints)
        out_name = path.stem + "_probe.json"
        out_path = args.out / out_name
        out_path.write_text(json.dumps(payload, indent=2), encoding="utf-8")
        manifest["files"][rel] = str(out_path.relative_to(ROOT))
        print(
            f"[lui]     {rel}: P={payload['p_widget_count']} Q={payload['q_stats']['q_record_count']} "
            f"→ {out_path.name}"
        )

    manifest_path = args.out / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    print(f"[lui]     manifest → {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
