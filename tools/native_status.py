#!/usr/bin/env python3
"""Summarize native port vs PS2 decomp (linked TUs, boot units, placeholders)."""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
MANIFEST = ROOT / "config" / "native_decomp_sources.txt"
UNITS = ROOT / "src/mem/units"
BOOT_KEYS = [
    "rom21_SYNCTASK_init",
    "rom21_func_003E6448",
    "rom21_SYNCTASK_add",
    "rom21_SYNCTASK_del",
    "rom21_SYNCTASK_run",
    "dcd10_cAppMan_setNextModule",
    "dcd10_cAppMan_run",
    "dcd10_cAppMan_mainLoop",
]


def count_placeholders() -> int:
    n = 0
    for cpp in SRC.rglob("*.cpp"):
        if cpp.is_relative_to(SRC / "platform"):
            continue
        text = cpp.read_text(encoding="utf-8", errors="replace").strip()
        if text.startswith("//Known file in project"):
            n += 1
    return n


def count_skip_asm_ready() -> int:
    n = 0
    for cpp in SRC.rglob("*.cpp"):
        rel = cpp.relative_to(ROOT).as_posix()
        if rel.startswith("src/platform/linux/") or rel.startswith("src/mem/units/"):
            continue
        text = cpp.read_text(encoding="utf-8", errors="replace")
        if "#ifdef SKIP_ASM" not in text:
            continue
        pos = 0
        while True:
            start = text.find("#ifdef SKIP_ASM", pos)
            if start < 0:
                break
            i = start + len("#ifdef SKIP_ASM")
            depth = 1
            while depth > 0:
                m = re.search(r"#(ifdef|ifndef|if|else|elif|endif)\b", text[i:])
                if not m:
                    break
                if m.group(1) == "endif":
                    depth -= 1
                elif m.group(1) in ("ifdef", "ifndef", "if"):
                    depth += 1
                i += m.end()
            block = text[start + len("#ifdef SKIP_ASM") : i - len("#endif")]
            stripped = re.sub(r"//.*?$|/\*.*?\*/", "", block, flags=re.MULTILINE | re.DOTALL).strip()
            if len(stripped) > 40:
                n += 1
                break
            pos = i
    return n


def main() -> int:
    linked: list[str] = []
    if MANIFEST.is_file():
        for line in MANIFEST.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line and not line.startswith("#"):
                linked.append(line)

    tagged = list(UNITS.glob("*.c"))
    matched = sum(1 for u in tagged if "// @objdiff-matched" in u.read_text(encoding="utf-8", errors="replace"))

    print("SSX3 native decomp status\n")
    print(f"  Linked game TUs (native):     {len(linked)}")
    print(f"  objdiff @objdiff-matched:     {matched} units in src/mem/units/")
    print(f"  src/**/*.cpp w/ SKIP_ASM>40:  {count_skip_asm_ready()} (candidates for sync_native_decomp.py)")
    print(f"  Placeholder //Known files:    {count_placeholders()}")
    print()
    print("  Boot-critical units:")
    for key in BOOT_KEYS:
        u = UNITS / f"{key}.c"
        tag = "MATCH" if u.is_file() and "// @objdiff-matched" in u.read_text(encoding="utf-8", errors="replace") else "—"
        print(f"    {tag:5}  {key}")
    print()
    print("  Native run modes:")
    print("    ./out/ssx3-native disc              smoke + disc check")
    print("    ./out/ssx3-native --boot-game disc  retail boot HAL loop")
    print("    ./out/ssx3-native --gfx disc        Vulkan FE menu (host)")
    print("    ./out/ssx3-native --game disc       procedural flyover + stubs")
    print()
    print("  Next: python3 scripts/sync_native_decomp.py && ninja native")
    print("  Doc:  docs/NATIVE_DECOMP.md")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
