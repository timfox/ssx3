#!/usr/bin/env python3
"""Export host FE layout JSON (retail defaults + fe.lui P-widget hints)."""

from __future__ import annotations

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROBE = ROOT / "assets" / "host" / "ui" / "lui_probe" / "fe_probe.json"
OUT = ROOT / "assets" / "host" / "ui" / "fe_layout.json"

# PS2 NTSC 640×448 — matches include/platform/host_fe_layout.h
MAIN_MENU = {
    "header_divider_y": 50.0,
    "column_divider_x": 272.0,
    "orange_three_x": 12.0,
    "orange_three_y": 8.0,
    "header_title_x": 20.0,
    "header_title_y": 14.0,
    "header_title_scale": 0.82,
    "corner_logo_w": 92.0,
    "corner_logo_x": 538.0,
    "corner_logo_y": 12.0,
    "desc_x": 24.0,
    "desc_y": 62.0,
    "desc_scale": 1.02,
    "menu_x": 292.0,
    "menu_y": 78.0,
    "menu_line_h": 34.0,
    "menu_scale": 1.36,
    "menu_scale_sel": 1.46,
    "sel_bar_x": 278.0,
    "sel_bar_w": 352.0,
    "sel_bar_h": 32.0,
    "mountain_x": 18.0,
    "mountain_y": 132.0,
    "mountain_w": 232.0,
    "mountain_h": 178.0,
    "hint_x": 468.0,
    "hint_y": 412.0,
    "hint_scale": 0.88,
}

MOUNTAIN = {
    "header_scale": 2.05,
    "header_x": 32.0,
    "header_y": 22.0,
    "peak_scale": 1.48,
    "peak_scale_sel": 1.78,
    "peak_y": 328.0,
    "peak_card_w": 152.0,
    "peak_gap": 14.0,
    "map_panorama_x": -24.0,
    "map_panorama_y": 40.0,
    "map_panorama_w": 688.0,
    "map_panorama_h": 340.0,
}

TITLE = {
    "logo_w": 548.0,
    "logo_y": 86.0,
    "press_scale": 1.62,
    "press_y": 296.0,
    "copyright_scale": 0.98,
    "copyright_y": 418.0,
    "mountain_x": -48.0,
    "mountain_y": 118.0,
    "mountain_w": 390.0,
    "mountain_h": 250.0,
    "fog_y": 252.0,
}


def lui_hints() -> dict[str, object]:
    if not PROBE.is_file():
        return {}
    probe = json.loads(PROBE.read_text(encoding="utf-8"))
    widgets = probe.get("p_widgets") or []
    list_rows = [
        w
        for w in widgets
        if w.get("x") == 260 and w.get("y") == 20 and isinstance(w.get("h"), int)
    ]
    list_rows.sort(key=lambda w: w["h"])
    return {
        "fe_lui_list_template": {
            "x": 260,
            "y": 20,
            "w": 348,
            "row_heights": [w["h"] for w in list_rows],
            "note": "P-widget templates in fe.lui (not yet mapped to main menu list)",
        }
    }


def main() -> int:
    payload = {
        "version": 1,
        "virtual_size": [640, 448],
        "title": TITLE,
        "main_menu": MAIN_MENU,
        "mountain_room": MOUNTAIN,
        "peak_map_textures": [
            "map_peakA_peak.png",
            "map_peakB_peak.png",
            "map_peakC_peak.png",
        ],
        "lui_hints": lui_hints(),
    }
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    print(f"[layout]  → {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
