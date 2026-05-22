#!/usr/bin/env python3
"""Print decompilation coverage toward 100% matching game code (NTSC-U main ELF)."""

from __future__ import annotations

import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
YAML = ROOT / "config/ssx3_us.yaml"
TAIL_ASM = ROOT / "asm/mem/rom_21e5a8_tail.s"
EARLY_MONOLITH = ROOT / "asm/1218.s"
EARLY_TAIL = ROOT / "asm/1218_tail.s"
EARLY_TAIL_ALT = ROOT / "asm/asm/1218_tail.s"
NATIVE_MANIFEST = ROOT / "config/native_decomp_sources.txt"

EARLY_START = 0x1218
EARLY_END = 0x1DC6D8
DCD10_START = 0x1DCD10
DCD10_END = 0x218618
ROM21_START = 0x21E5A8
ROM21_END = 0x2EC198
LATE2_START = 0x2EEF50
LATE2_END = 0x32F590
MAIN_START = 0x1000
LATE_TEXT_END = 0x32F590


def parse_tail_functions(path: Path) -> int:
    if not path.is_file():
        return 0
    text = path.read_text(encoding="utf-8", errors="replace")
    return len(re.findall(r"^glabel ", text, re.M))


def count_ps2_stubs() -> int:
    return len(list((ROOT / "src/mem").glob("rom21_*_ps2.s")))


def count_yaml_segments(pattern: str) -> int:
    text = YAML.read_text(encoding="utf-8", errors="replace")
    return len(re.findall(pattern, text))


def tail_yaml_offset() -> int | None:
    text = YAML.read_text(encoding="utf-8", errors="replace")
    m = re.search(r"- \[(0x[0-9A-Fa-f]+), asm, mem/rom_21e5a8_tail\]", text)
    return int(m.group(1), 16) if m else None


def monolith_tail_offset(tail_name: str) -> int | None:
    text = YAML.read_text(encoding="utf-8", errors="replace")
    m = re.search(rf"- \[(0x[0-9A-Fa-f]+), asm, {re.escape(tail_name)}\]", text)
    return int(m.group(1), 16) if m else None


def bare_monolith_offset(hex_addr: int) -> bool:
    text = YAML.read_text(encoding="utf-8", errors="replace")
    return bool(re.search(rf"- \[0x{hex_addr:06X}, asm\]\s*$", text, re.M))


def early_tail_yaml_offset() -> int | None:
    text = YAML.read_text(encoding="utf-8", errors="replace")
    m = re.search(r"- \[(0x[0-9A-Fa-f]+), asm, 1218_tail\]", text)
    if m:
        return int(m.group(1), 16)
    if re.search(r"- \[0x1218, asm\]\s*$", text, re.M):
        return EARLY_START
    return None


def object_c_units_progress() -> tuple[int, int]:
    units_dir = ROOT / "src/mem/units"
    if not units_dir.is_dir():
        return 0, 0
    units = list(units_dir.glob("*.cpp")) + list(units_dir.glob("*.c"))
    matched = sum(
        1
        for p in units
        if "@objdiff-matched" in p.read_text(encoding="utf-8", errors="replace")
    )
    return len(units), matched


def native_skip_asm_sources() -> list[str]:
    if not NATIVE_MANIFEST.is_file():
        return []
    return [
        line.strip()
        for line in NATIVE_MANIFEST.read_text().splitlines()
        if line.startswith("src/")
    ]


def pct(num: float, den: float) -> str:
    if den <= 0:
        return "n/a"
    return f"{100.0 * num / den:.1f}%"


def main() -> int:
    tail_off = tail_yaml_offset()
    rom21_has_tail = count_yaml_segments(r"rom_21e5a8_tail") > 0
    tail_funcs = parse_tail_functions(TAIL_ASM) if rom21_has_tail else 0
    rom21_split = count_yaml_segments(r"mem/rom21_")

    rom21_bytes = ROM21_END - ROM21_START
    rom21_done_bytes = (tail_off - ROM21_START) if tail_off else 0
    rom21_remain_bytes = ROM21_END - tail_off if tail_off else rom21_bytes

    early_bytes = EARLY_END - EARLY_START
    early_tail_off = early_tail_yaml_offset()
    early_has_tail = early_tail_off is not None
    tail_asm = EARLY_TAIL if EARLY_TAIL.is_file() else (
        EARLY_TAIL_ALT if EARLY_TAIL_ALT.is_file() else None
    )
    early_tail_funcs = parse_tail_functions(tail_asm) if tail_asm else 0
    early_split = count_yaml_segments(r"early/early_")
    early_done = (early_tail_off - EARLY_START) if early_tail_off else 0
    early_remain = EARLY_END - early_tail_off if early_tail_off else early_bytes

    late_bytes = LATE_TEXT_END - ROM21_END

    ps2_stubs = count_ps2_stubs()
    early_ps2 = len(list((ROOT / "src/early").glob("early_*_ps2.s"))) if (ROOT / "src/early").is_dir() else 0
    native_sources = native_skip_asm_sources()

    print("SSX3 decompilation progress (100% = every .text unit objdiff-matched)")
    print()
    print("## Definition of done (PS2 matching build)")
    print("  1. Every function in its own splat segment (no giant tail blobs).")
    print("  2. obj/target/*.o vs obj/current/*.o byte-identical for each unit.")
    print("  3. obj/current uses C (#ifdef SKIP_ASM) or verified *_ps2.s — not drift.")
    print("  4. Optional later: native port links all game TUs (separate milestone).")
    print()
    print("## Main .text regions (file offsets)")
    print(f"  Early 0x1218      0x{EARLY_START:X}..0x{EARLY_END:X}  {early_bytes // 1024:5} KiB")
    if early_has_tail:
        print(f"    split through 0x{early_tail_off:X}  {early_done // 1024:5} KiB  ({pct(early_done, early_bytes)})")
        print(f"    tail left       {early_remain // 1024:5} KiB  ({pct(early_remain, early_bytes)})")
    elif early_split:
        print(f"    early split:              DONE")
    print(f"  Heap/crowd etc  0x{MAIN_START:X}..0x{EARLY_START:X}  {(EARLY_START - MAIN_START) // 1024:5} KiB")
    print(f"  rom21 region    0x{ROM21_START:X}..0x{ROM21_END:X}  {rom21_bytes // 1024:5} KiB")
    if tail_off:
        print(f"    split through 0x{tail_off:X}  {rom21_done_bytes // 1024:5} KiB  ({pct(rom21_done_bytes, rom21_bytes)})")
        print(f"    tail left       {rom21_remain_bytes // 1024:5} KiB  ({pct(rom21_remain_bytes, rom21_bytes)})")
    print(f"  Late text       0x{ROM21_END:X}..0x{LATE_TEXT_END:X}  {late_bytes // 1024:5} KiB  (dirtysock + tail asm)")
    print(f"  .vutext         0x32F590+               (VU microcode — match separately)")
    print()
    print("## rom21 per-function split (automated tail tool)")
    est_total = rom21_split + tail_funcs if rom21_has_tail else rom21_split
    print(f"  Named rom21 yaml segments: {rom21_split}")
    if rom21_has_tail:
        print(f"  Functions still in tail:   {tail_funcs}")
        print(f"  Estimated rom21 total:     ~{est_total}")
        if est_total:
            print(f"  rom21 function progress:   {rom21_split}/{est_total} ({pct(rom21_split, est_total)})")
    else:
        print("  rom21 tail:                DONE (no rom_21e5a8_tail in yaml)")
        print(f"  rom21 function progress:   {rom21_split}/{rom21_split} (100%)")
    print(f"  rom21 *_ps2.s stubs:       {ps2_stubs}")
    if tail_funcs:
        batches_50 = (tail_funcs + 49) // 50
        print(f"  Tail drain (~50/batch):    ~{batches_50} runs of split_rom21_tail.py")
    print()
    print("## early 0x1218 per-function split")
    est_early = early_split + early_tail_funcs if early_has_tail else early_split
    print(f"  Named early yaml segments: {early_split}")
    if early_has_tail:
        print(f"  Functions still in tail:   {early_tail_funcs}")
        print(f"  Estimated early total:     ~{est_early}")
        if est_early:
            print(f"  early function progress:   {early_split}/{est_early} ({pct(early_split, est_early)})")
    else:
        print("  early tail:                DONE")
        print(f"  early function progress:   {early_split}/{early_split} (100%)")
    print(f"  early *_ps2.s stubs:       {early_ps2}")
    if early_tail_funcs:
        print(f"  1218 drain (~100/batch):   ~{(early_tail_funcs + 99) // 100} runs of split_1218_tail.py")
    print()
    print("## C / native port")
    print(f"  native_decomp_sources.txt: {len(native_sources)} TUs")
    for src in native_sources:
        print(f"    - {src}")
    print()
    bx218 = count_yaml_segments(r"bx218/")
    heapgap = count_yaml_segments(r"heapgap/")
    dcd10_split = count_yaml_segments(r"dcd10/dcd10_")
    dcd10_tail_off = monolith_tail_offset("1dcd10_tail")
    dcd10_has_tail = dcd10_tail_off is not None
    dcd10_tail_asm = ROOT / "asm/1dcd10_tail.s"
    dcd10_tail_funcs = parse_tail_functions(dcd10_tail_asm) if dcd10_has_tail and dcd10_tail_asm.is_file() else 0
    dcd10_ps2 = len(list((ROOT / "src/dcd10").glob("dcd10_*_ps2.s"))) if (ROOT / "src/dcd10").is_dir() else 0
    est_dcd10 = dcd10_split + dcd10_tail_funcs if dcd10_has_tail else dcd10_split

    late_split = count_yaml_segments(r"late/late_")
    late2_bare = bare_monolith_offset(LATE2_START)
    late2_tail_off = monolith_tail_offset("2eef50_tail")
    late2_has_tail = late2_tail_off is not None
    late2_tail_asm = ROOT / "asm/2eef50_tail.s"
    late2_tail_funcs = (
        parse_tail_functions(late2_tail_asm) if late2_has_tail and late2_tail_asm.is_file() else 0
    )
    late2_ps2 = len(list((ROOT / "src/late").glob("late_*_ps2.s"))) if (ROOT / "src/late").is_dir() else 0
    est_late2 = late_split + late2_tail_funcs if late2_has_tail else late_split

    print("## Other monolith splits")
    print(f"  bx218 (0x218ae8..0x218d20):  {bx218}/5 {'DONE' if bx218 >= 5 else 'in progress'}")
    print(f"  heapgap (0x21a1c0..0x21aa90): {heapgap}/16 {'DONE' if heapgap >= 16 else 'in progress'}")
    print()
    print("## 0x1dcd10 per-function split")
    print(f"  Named dcd10 yaml segments: {dcd10_split}")
    if dcd10_has_tail:
        dcd10_done = dcd10_tail_off - DCD10_START
        dcd10_bytes = DCD10_END - DCD10_START
        print(f"  Tail offset:               0x{dcd10_tail_off:X} ({pct(dcd10_done, dcd10_bytes)} of region)")
        print(f"  Functions still in tail:   {dcd10_tail_funcs}")
        print(f"  Estimated dcd10 total:     ~{est_dcd10}")
        if est_dcd10:
            print(f"  dcd10 function progress:   {dcd10_split}/{est_dcd10} ({pct(dcd10_split, est_dcd10)})")
        if dcd10_tail_funcs:
            print(f"  1dcd10 drain (~100/batch): ~{(dcd10_tail_funcs + 99) // 100} runs left")
    elif dcd10_split:
        print("  1dcd10 tail:               DONE")
        print(f"  dcd10 function progress:   {dcd10_split}/{dcd10_split} (100%)")
    print(f"  dcd10 *_ps2.s stubs:       {dcd10_ps2}")
    print()
    print("## 0x2eef50 late monolith")
    if late2_bare:
        print("  Status:                    NOT STARTED (bare [0x2eef50, asm])")
        print("  Estimated functions:       ~1419")
    elif late2_has_tail:
        late2_done = late2_tail_off - LATE2_START
        late2_bytes = LATE2_END - LATE2_START
        print(f"  Named late yaml segments:  {late_split}")
        print(f"  Tail offset:               0x{late2_tail_off:X} ({pct(late2_done, late2_bytes)} of region)")
        print(f"  Functions still in tail:   {late2_tail_funcs}")
        print(f"  Estimated late total:      ~{est_late2}")
        if est_late2:
            print(f"  late function progress:    {late_split}/{est_late2} ({pct(late_split, est_late2)})")
        if late2_tail_funcs:
            print(f"  2eef50 drain (~100/batch): ~{(late2_tail_funcs + 99) // 100} runs left")
    elif late_split:
        print("  2eef50 tail:               DONE")
        print(f"  late function progress:    {late_split}/{late_split} (100%)")
    print(f"  late *_ps2.s stubs:        {late2_ps2}")
    print()
    unit_total, unit_matched = object_c_units_progress()
    print("## Object C units (rom21 SKIP_ASM → src/mem/units)")
    print(f"  Unit files:                {unit_total}")
    print(f"  objdiff-matched (@tag):    {unit_matched}/{unit_total}")
    if unit_total and unit_matched < unit_total:
        print("  obj/current uses *_ps2.s until a unit is tagged @objdiff-matched")
    print("  Verify/tag:                python3 tools/batch_verify_units.py")
    print("  Regenerate:                python3 tools/extract_object_c_units.py")
    print("## Still monolithic in yaml")
    bare = len(re.findall(r", asm\]\s*$", YAML.read_text(), re.M))
    print(f"  Unnamed asm subsegments:   {bare}")
    print()
    print("## objdiff .text (global)")
    print("  Run: python3 tools/objdiff_progress.py")
    print("  Boot units: python3 tools/generate_ps2_units.py --boot")
    print()
    print("## Native playable milestones")
    print("  1. objdiff: per-function .text match (15000+ units)")
    print("  2. Readable C: SKIP_ASM bodies + sync_native_decomp.py")
    print("  3. HAL: host_* replaces EE syscalls (boot, pad, disc, GS)")
    print("  4. ./out/ssx3-native --boot-game disc  (retail tick loop)")
    print("  5. ./out/ssx3-native --gfx disc         (host FE menu)")
    print("  6. Race/world: decomp cWorld_*, render, physics (long-term)")
    print()
    print("## Commands")
    print("  python3 tools/decomp_progress.py")
    print("  python3 tools/split_rom21_tail.py --batch 50   # rom21 done")
    print("  python3 tools/split_1218_tail.py --batch 100 --until-empty")
    print("  python3 tools/split_monolith.py --asm asm/2EEF50.s --tail-name 2eef50_tail \\")
    print("    --prefix late_ --region-start 0x2eef50 --region-end 0x32f590 \\")
    print("    --yaml-subpath late --ps2-dir src/late \\")
    print("    --configure-anchor rom21_func_003EB190 --batch 100 --until-empty")
    print("  venv/bin/python configure.py -o && ninja       # objdiff PS2 build")
    print("  python3 scripts/sync_native_decomp.py && ninja native")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
