#!/usr/bin/env python3
"""Split rom_21e5a8_tail.s into per-function yaml/configure/ps2 segments."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TAIL_ASM = ROOT / "asm/mem/rom_21e5a8_tail.s"
YAML_PATH = ROOT / "config/ssx3_us.yaml"
CONFIGURE_PATH = ROOT / "configure.py"
ROM21_END = 0x2EC198


def remove_empty_tail_yaml_entry() -> None:
    text = YAML_PATH.read_text()
    line = re.search(r"\n      - \[0x[0-9A-Fa-f]+, asm, mem/rom_21e5a8_tail\]\n", text)
    if line:
        YAML_PATH.write_text(text.replace(line.group(0), "\n", 1))


def parse_tail(path: Path) -> list[tuple[str, int, int]]:
    text = path.read_text()
    funcs: list[tuple[str, int, int]] = []
    for m in re.finditer(
        r"nonmatching (\S+), (0x[0-9A-Fa-f]+)\s*\n\nglabel (\S+)", text
    ):
        glabel, size = m.group(3), int(m.group(2), 16)
        rm = re.search(
            rf"glabel {re.escape(glabel)}\s*\n\s*/\* ([0-9A-Fa-f]+)", text
        )
        if not rm:
            raise SystemExit(f"no ROM comment for {glabel}")
        funcs.append((glabel, size, int(rm.group(1), 16)))
    return funcs


def yaml_offset(rom: int) -> int:
    return rom


def segment_name(glabel: str) -> str:
    return f"rom21_{glabel}"


def apply_yaml(batch: list[tuple[str, int, int]], tail_yaml: int, dry_run: bool) -> None:
    text = YAML_PATH.read_text()
    anchor = f"      - [0x{tail_yaml:06X}, asm, mem/rom_21e5a8_tail]\n"
    if anchor not in text:
        raise SystemExit(f"yaml anchor missing: {anchor.strip()}")

    lines: list[str] = []
    for glabel, _size, rom in batch:
        seg = segment_name(glabel)
        lines.append(f"      - [0x{yaml_offset(rom):06X}, asm, mem/{seg}]\n")
    new_end = batch[-1][2] + batch[-1][1]
    lines.append(f"      - [0x{new_end:06X}, asm, mem/rom_21e5a8_tail]\n")
    replacement = "".join(lines)

    if dry_run:
        print("--- yaml ---")
        print(replacement)
        return
    YAML_PATH.write_text(text.replace(anchor, replacement, 1))
    print(f"yaml: {len(batch)} segments, tail now at 0x{new_end:06X}")


def apply_configure(batch: list[tuple[str, int, int]], tail_yaml: int, dry_run: bool) -> None:
    cfg = CONFIGURE_PATH.read_text()
    # Anchor on the segment immediately before this batch in yaml.
    text = YAML_PATH.read_text()
    anchor_seg = None
    for i, line in enumerate(text.splitlines()):
        if f"0x{yaml_offset(batch[0][2]):06X}" in line and segment_name(batch[0][0]) in line:
            if i > 0:
                m = re.search(r"mem/(rom21_[A-Za-z0-9_]+)", text.splitlines()[i - 1])
                if m:
                    anchor_seg = m.group(1)
            break
    if not anchor_seg:
        raise SystemExit("could not find configure anchor segment before batch")

    anchor = f'                "{anchor_seg}": Path("src/mem/{anchor_seg}_ps2.s"),\n'
    if anchor not in cfg:
        raise SystemExit(f"configure anchor missing: {anchor_seg}")

    insert_lines = []
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        insert_lines.append(
            f'                "{seg}": Path("src/mem/{seg}_ps2.s"),\n'
        )

    if dry_run:
        print("--- configure (first 3) ---")
        print("".join(insert_lines[:3]))
        return
    if batch[0][0] in cfg and f'"{segment_name(batch[0][0])}"' in cfg:
        print("configure: batch already present")
        return
    CONFIGURE_PATH.write_text(cfg.replace(anchor, anchor + "".join(insert_lines), 1))
    print(f"configure: +{len(batch)} ps2_asm_map entries")


def emit_ps2(batch: list[tuple[str, int, int]], skip_existing: bool) -> list[str]:
    ok: list[str] = []
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        asm = ROOT / "asm/mem" / f"{seg}.s"
        ps2 = ROOT / "src/mem" / f"{seg}_ps2.s"
        if not asm.exists():
            raise SystemExit(f"missing splat asm: {asm}")
        if skip_existing and ps2.exists():
            ok.append(seg)
            continue
        r = subprocess.run(
            [
                sys.executable,
                str(ROOT / "tools/gen_ps2_from_splat.py"),
                str(asm),
                glabel,
                str(ps2),
            ],
            capture_output=True,
            text=True,
        )
        if r.returncode:
            raise SystemExit(f"gen_ps2 failed {seg}: {r.stderr[:200]}")
        ok.append(seg)
    return ok


def verify_objects(batch: list[tuple[str, int, int]]) -> tuple[int, int]:
    matched = 0
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        target = ROOT / "obj/target" / f"{seg}.o"
        current = ROOT / "obj/current" / f"{seg}.o"
        if target.exists() and current.exists():
            r = subprocess.run(["cmp", "-s", str(target), str(current)], check=False)
            if r.returncode == 0:
                matched += 1
    return matched, len(batch)


def run_once(args: argparse.Namespace) -> int:
    funcs = parse_tail(TAIL_ASM)
    if not funcs:
        print("tail empty — nothing to split")
        return 0

    text = YAML_PATH.read_text()
    m = re.search(r"- \[(0x[0-9A-Fa-f]+), asm, mem/rom_21e5a8_tail\]", text)
    if not m:
        print("rom_21e5a8_tail yaml entry not found — tail fully split")
        return 0
    tail_yaml = int(m.group(1), 16)
    if tail_yaml >= ROM21_END:
        remove_empty_tail_yaml_entry()
        print(f"removed empty rom_21e5a8_tail at 0x{tail_yaml:06X}")
        return 0

    start_idx = 0
    for i, (_g, _s, rom) in enumerate(funcs):
        if yaml_offset(rom) == tail_yaml:
            start_idx = i
            break
    else:
        if tail_yaml >= ROM21_END:
            remove_empty_tail_yaml_entry()
            print(f"removed empty rom_21e5a8_tail at 0x{tail_yaml:06X}")
            return 0
        raise SystemExit(f"no function at tail yaml 0x{tail_yaml:06X}")

    batch = funcs[start_idx : start_idx + args.batch]
    if not batch:
        print("tail empty — nothing to split")
        return 0

    print(
        f"split {len(batch)} @ yaml 0x{tail_yaml:06X}: "
        f"{batch[0][0]} .. {batch[-1][0]}"
    )

    apply_yaml(batch, tail_yaml, args.dry_run)
    apply_configure(batch, tail_yaml, args.dry_run)
    if args.dry_run or args.yaml_only:
        return 0

    if not args.skip_build:
        r = subprocess.run(
            [str(ROOT / "venv/bin/python"), str(ROOT / "configure.py"), "-o"],
            cwd=ROOT,
            check=False,
        )
        if r.returncode:
            return r.returncode

    if not args.skip_ps2:
        segs = emit_ps2(batch, skip_existing=False)
        print(f"ps2: {len(segs)} files")
        r = subprocess.run(
            [sys.executable, str(ROOT / "tools/sync_rom21_ps2.py")],
            cwd=ROOT,
            check=False,
        )
        if r.returncode:
            return r.returncode

    if not args.skip_build:
        r = subprocess.run(["ninja"], cwd=ROOT, check=False)
        if r.returncode:
            return r.returncode

    matched, total = verify_objects(batch)
    print(f"objdiff bytes: {matched}/{total} match")
    remaining = len(funcs) - (start_idx + len(batch))
    print(f"tail remainder: ~{remaining} functions")
    return 0 if matched == total else 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--batch", type=int, default=50, help="functions per split")
    ap.add_argument(
        "--until-empty",
        action="store_true",
        help="repeat --batch splits until rom_21e5a8_tail is gone",
    )
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--yaml-only", action="store_true")
    ap.add_argument("--skip-build", action="store_true")
    ap.add_argument("--skip-ps2", action="store_true")
    args = ap.parse_args()

    if not TAIL_ASM.is_file():
        raise SystemExit(f"missing tail asm: {TAIL_ASM}")

    if args.until_empty:
        if args.dry_run:
            raise SystemExit("--until-empty cannot be used with --dry-run")
        round_idx = 0
        while re.search(r"rom_21e5a8_tail", YAML_PATH.read_text()):
            if not parse_tail(TAIL_ASM):
                print("tail asm empty")
                break
            round_idx += 1
            print(f"=== tail split round {round_idx} ===")
            rc = run_once(args)
            if rc != 0:
                return rc
        print("tail fully split")
        return 0

    return run_once(args)


if __name__ == "__main__":
    raise SystemExit(main())
