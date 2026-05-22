"""Extract UTF-16 strings from EA LOCH .loc files."""

from __future__ import annotations

import json
import re
from pathlib import Path


def extract_utf16_strings(data: bytes, min_len: int = 2) -> list[dict[str, object]]:
    strings: list[dict[str, object]] = []
    seen: set[str] = set()
    for match in re.finditer(rb"(?:[\x20-\x7e]\x00)+", data):
        text = match.group().decode("utf-16-le")
        if len(text) < min_len or text in seen:
            continue
        seen.add(text)
        strings.append({"offset": match.start(), "text": text})
    return strings


def extract_loc_file(path: Path) -> dict[str, object]:
    data = path.read_bytes()
    magic = data[:4].decode("ascii", errors="replace")
    return {
        "file": path.name,
        "magic": magic,
        "size": len(data),
        "strings": extract_utf16_strings(data),
    }


def find_string(strings: list[dict[str, object]], exact: str) -> str | None:
    for entry in strings:
        if entry.get("text") == exact:
            return str(entry["text"])
    return None


# Curated retail NTSC strings for the native FE stand-in.
FE_STRING_KEYS: dict[str, tuple[str, str]] = {
    "fe.title.press_start": ("feamer.loc", "Press START button"),
    "fe.title.copyright": (
        "feamer.loc",
        " 2003 Electronic Arts Inc. All Rights Reserved.",
    ),
    "fe.main_menu.title": ("cmnamer.loc", "Main Menu"),
    "fe.main_menu.single_event": ("cmnamer.loc", "Single Event"),
    "fe.main_menu.conquer_mountain": ("cmnamer.loc", "Conquer The Mountain"),
    "fe.main_menu.multi_play": ("cmnamer.loc", "Multi Play"),
    "fe.main_menu.previews": ("cmnamer.loc", "Previews"),
    "fe.main_menu.online": ("cmnamer.loc", "Online"),
    "fe.main_menu.single_event.desc": ("feamer.loc", "Play any unlocked Single Event."),
    "fe.main_menu.conquer_mountain.desc": (
        "feamer.loc",
        "Continue to peak selection.",
    ),
    "fe.main_menu.multi_play.desc": ("feamer.loc", "2 player head to head competition."),
    "fe.main_menu.previews.desc": ("feamer.loc", "View art, posters, cards, and videos earned."),
    "fe.main_menu.online.desc": ("feamer.loc", "Compete against SSX 3 players around the world."),
    "fe.hints.select": ("cmnamer.loc", "Select"),
    "fe.hints.back": ("cmnamer.loc", "Back"),
    "fe.hints.previous": ("feamer.loc", "Previous Screen"),
    "fe.hints.options": ("cmnamer.loc", "Options"),
    "fe.options.title": ("cmnamer.loc", "Options"),
    "fe.options.game": ("cmnamer.loc", "Game Options"),
    "fe.options.sound": ("cmnamer.loc", "Sound Options"),
    "fe.options.controller": ("cmnamer.loc", "Controller Settings"),
    "fe.options.save_load": ("cmnamer.loc", "Save/Load"),
    "fe.options.back": ("cmnamer.loc", "Back"),
    "fe.mountain.title": ("cmnamer.loc", "Freeride"),
    "fe.mountain.peak1": ("cmnamer.loc", "Peak 1"),
    "fe.mountain.peak2": ("cmnamer.loc", "Peak 2"),
    "fe.mountain.peak3": ("cmnamer.loc", "Peak 3"),
    "fe.mountain.select_peak": ("cmnamer.loc", "Select Peak"),
    "fe.previews.title": ("cmnamer.loc", "Previews"),
    "fe.previews.posters": ("cmnamer.loc", "Posters"),
    "fe.previews.trading_cards": ("cmnamer.loc", "Trading Cards"),
    "fe.previews.videos": ("cmnamer.loc", "Videos"),
    "fe.previews.cheat_characters": ("cmnamer.loc", "Cheat Characters"),
    "fe.multiplay.title": ("feamer.loc", "Select Multi Play Mode"),
    "fe.multiplay.two_player": ("cmnamer.loc", "2 Player"),
    "fe.single_event.title": ("feamer.loc", "Single Event"),
    "fe.single_event.big_air": ("cmnamer.loc", "Big Air"),
    "fe.single_event.race": ("cmnamer.loc", "Race"),
    "fe.single_event.slopestyle": ("cmnamer.loc", "Slopestyle"),
    "fe.single_event.backcountry": ("cmnamer.loc", "Backcountry"),
}

# Host-only copy (not present verbatim in retail .loc files).
FE_HOST_STRINGS: dict[str, str] = {
    "fe.multiplay.multiplayer": "Multiplayer",
    "fe.options.controller.body": (
        "Controller remapping and vibration (retail cFEStateOptionsController @ 0x18C6A0) "
        "is not ported on the host build yet."
    ),
    "fe.options.saveload.body": (
        "Memory card / profile save and load (retail cFEStateOptionsSaveLoad @ 0x18D280) "
        "is not ported on the host build yet."
    ),
}


def build_string_map(loc_dir: Path) -> tuple[dict[str, str], list[str]]:
    blobs: dict[str, list[dict[str, object]]] = {}
    for path in sorted(loc_dir.glob("*.loc")):
        blobs[path.name] = extract_loc_file(path)["strings"]  # type: ignore[assignment]

    out: dict[str, str] = {}
    warnings: list[str] = []
    for key, (loc_name, exact) in FE_STRING_KEYS.items():
        pool = blobs.get(loc_name)
        if pool is None:
            warnings.append(f"{key}: missing {loc_name}")
            out[key] = exact
            continue
        hit = find_string(pool, exact)
        if hit is None:
            warnings.append(f"{key}: {exact!r} not in {loc_name} (using curated text)")
            out[key] = exact
            continue
        out[key] = hit
    out.update(FE_HOST_STRINGS)
    return out, warnings
