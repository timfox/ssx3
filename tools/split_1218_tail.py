#!/usr/bin/env python3
"""Split asm/1218.s monolith into per-function yaml/configure/ps2 segments."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MONOLITH_ASM = ROOT / "asm/1218.s"
TAIL_ASM = ROOT / "asm/1218_tail.s"
TAIL_ASM_LEGACY = ROOT / "asm/asm/1218_tail.s"
YAML_PATH = ROOT / "config/ssx3_us.yaml"
CONFIGURE_PATH = ROOT / "configure.py"
REGION_START = 0x1218
REGION_END = 0x1DC6D8
TAIL_YAML_NAME = "1218_tail"
CONFIGURE_FIRST_ANCHOR = "crowd_anim2d_update"


def parse_monolith(path: Path) -> list[tuple[str, int, int]]:
    text = path.read_text(encoding="utf-8", errors="replace")
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


def segment_name(glabel: str) -> str:
    return f"early_{glabel}"


def yaml_offset(file_off: int) -> int:
    return file_off


def tail_anchor_line(tail_yaml: int) -> str:
    return f"      - [0x{tail_yaml:06X}, asm, {TAIL_YAML_NAME}]\n"


def find_tail_yaml(text: str) -> int | None:
    for name in (TAIL_YAML_NAME, "asm/1218_tail"):
        m = re.search(rf"- \[(0x[0-9A-Fa-f]+), asm, {re.escape(name)}\]", text)
        if m:
            return int(m.group(1), 16)
    m = re.search(r"- \[0x1218, asm\]\s*\n", text)
    if m:
        return REGION_START
    return None


def tail_asm_path() -> Path:
    if TAIL_ASM.is_file():
        return TAIL_ASM
    if TAIL_ASM_LEGACY.is_file():
        return TAIL_ASM_LEGACY
    return MONOLITH_ASM


def remove_empty_tail_yaml_entry() -> None:
    text = YAML_PATH.read_text(encoding="utf-8")
    line = re.search(
        rf"\n      - \[0x[0-9A-Fa-f]+, asm, {re.escape(TAIL_YAML_NAME)}\]\n",
        text,
    )
    if line:
        YAML_PATH.write_text(text.replace(line.group(0), "\n", 1))


def apply_yaml(batch: list[tuple[str, int, int]], tail_yaml: int, dry_run: bool) -> None:
    text = YAML_PATH.read_text(encoding="utf-8")
    if tail_yaml == REGION_START:
        anchor = "      - [0x1218, asm]\n"
    else:
        anchor = tail_anchor_line(tail_yaml)
    if anchor not in text:
        raise SystemExit(f"yaml anchor missing: {anchor.strip()}")

    lines: list[str] = []
    for glabel, _size, rom in batch:
        seg = segment_name(glabel)
        lines.append(f"      - [0x{yaml_offset(rom):06X}, asm, early/{seg}]\n")
    new_end = batch[-1][2] + batch[-1][1]
    lines.append(tail_anchor_line(new_end))
    replacement = "".join(lines)

    if dry_run:
        print("--- yaml ---")
        print(replacement[:800])
        return
    YAML_PATH.write_text(text.replace(anchor, replacement, 1))
    print(f"yaml: {len(batch)} segments, tail now at 0x{new_end:06X}")


def configure_anchor_seg(batch: list[tuple[str, int, int]]) -> str:
    text = YAML_PATH.read_text(encoding="utf-8")
    for i, line in enumerate(text.splitlines()):
        if segment_name(batch[0][0]) in line and f"0x{yaml_offset(batch[0][2]):06X}" in line:
            if i > 0:
                m = re.search(r"early/(early_[A-Za-z0-9_]+)", text.splitlines()[i - 1])
                if m:
                    return m.group(1)
            break
    return CONFIGURE_FIRST_ANCHOR


def apply_configure(batch: list[tuple[str, int, int]], dry_run: bool) -> None:
    cfg = CONFIGURE_PATH.read_text(encoding="utf-8")
    anchor_seg = configure_anchor_seg(batch)
    anchor = f'                "{anchor_seg}": Path("src/visualfx/{anchor_seg}_ps2.s"),\n'
    if anchor_seg.startswith("early_"):
        anchor = f'                "{anchor_seg}": Path("src/early/{anchor_seg}_ps2.s"),\n'
    if anchor not in cfg:
        raise SystemExit(f"configure anchor missing: {anchor_seg}")

    insert_lines = []
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        insert_lines.append(
            f'                "{seg}": Path("src/early/{seg}_ps2.s"),\n'
        )

    if dry_run:
        print("--- configure (first 3) ---")
        print("".join(insert_lines[:3]))
        return
    if f'"{segment_name(batch[0][0])}"' in cfg:
        print("configure: batch already present")
        return
    CONFIGURE_PATH.write_text(cfg.replace(anchor, anchor + "".join(insert_lines), 1))
    print(f"configure: +{len(batch)} ps2_asm_map entries (anchor {anchor_seg})")


def emit_ps2(batch: list[tuple[str, int, int]], skip_existing: bool) -> list[str]:
    ok: list[str] = []
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        asm = ROOT / "asm/early" / f"{seg}.s"
        ps2 = ROOT / "src/early" / f"{seg}_ps2.s"
        if not asm.is_file():
            raise SystemExit(f"missing splat asm: {asm}")
        ps2.parent.mkdir(parents=True, exist_ok=True)
        if skip_existing and ps2.is_file():
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


def sync_early_ps2() -> int:
    cfg = CONFIGURE_PATH.read_text(encoding="utf-8")
    segs = sorted(
        set(re.findall(r'"(early_[A-Za-z0-9_]+)": Path\("src/early/', cfg))
    )
    missing = [s for s in segs if not (ROOT / "src/early" / f"{s}_ps2.s").is_file()]
    if not missing:
        return 0
    print(f"sync_early_ps2: generating {len(missing)} stub(s)...")
    for seg in missing:
        glabel = seg[6:]  # early_func_... -> func_...
        asm = ROOT / "asm/early" / f"{seg}.s"
        ps2 = ROOT / "src/early" / f"{seg}_ps2.s"
        if not asm.is_file():
            print(f"  skip {seg}: no asm yet")
            continue
        subprocess.run(
            [sys.executable, str(ROOT / "tools/gen_ps2_from_splat.py"), str(asm), glabel, str(ps2)],
            check=True,
        )
    return 0


def verify_objects(batch: list[tuple[str, int, int]]) -> tuple[int, int]:
    matched = 0
    for glabel, _size, _rom in batch:
        seg = segment_name(glabel)
        target = ROOT / "obj/target" / f"{seg}.o"
        current = ROOT / "obj/current" / f"{seg}.o"
        if target.is_file() and current.is_file():
            r = subprocess.run(["cmp", "-s", str(target), str(current)], check=False)
            if r.returncode == 0:
                matched += 1
    return matched, len(batch)


def run_once(args: argparse.Namespace) -> int:
    funcs = parse_monolith(tail_asm_path())
    if not funcs:
        raise SystemExit("no functions in 1218.s")

    yaml_text = YAML_PATH.read_text(encoding="utf-8")
    tail_yaml = find_tail_yaml(yaml_text)
    if tail_yaml is None:
        print("1218 monolith fully split (no tail anchor in yaml)")
        return 0
    if tail_yaml >= REGION_END:
        remove_empty_tail_yaml_entry()
        print(f"removed empty {TAIL_YAML_NAME} at 0x{tail_yaml:06X}")
        return 0

    start_idx = 0
    for i, (_g, _s, rom) in enumerate(funcs):
        if yaml_offset(rom) == tail_yaml:
            start_idx = i
            break
    else:
        if tail_yaml >= REGION_END:
            remove_empty_tail_yaml_entry()
            print(f"removed empty {TAIL_YAML_NAME} at 0x{tail_yaml:06X}")
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
    apply_configure(batch, args.dry_run)
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
        sync_early_ps2()

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
    ap.add_argument("--until-empty", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--yaml-only", action="store_true")
    ap.add_argument("--skip-build", action="store_true")
    ap.add_argument("--skip-ps2", action="store_true")
    args = ap.parse_args()

    if not MONOLITH_ASM.is_file():
        raise SystemExit(f"missing monolith: {MONOLITH_ASM}")

    if args.until_empty:
        if args.dry_run:
            raise SystemExit("--until-empty cannot be used with --dry-run")
        round_idx = 0
        while find_tail_yaml(YAML_PATH.read_text(encoding="utf-8")) is not None:
            if not parse_monolith(tail_asm_path()):
                print("1218.s empty")
                break
            round_idx += 1
            print(f"=== 1218 split round {round_idx} ===", flush=True)
            rc = run_once(args)
            if rc != 0:
                return rc
        print("1218 monolith fully split")
        return 0

    return run_once(args)


if __name__ == "__main__":
    raise SystemExit(main())
