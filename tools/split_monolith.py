#!/usr/bin/env python3
"""Split a splat monolith .s blob into per-function yaml/configure/ps2 segments."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
YAML_PATH = ROOT / "config/ssx3_us.yaml"
CONFIGURE_PATH = ROOT / "configure.py"


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


def find_tail_yaml(text: str, tail_name: str, region_start: int) -> int | None:
    m = re.search(rf"- \[(0x[0-9A-Fa-f]+), asm, {re.escape(tail_name)}\]", text)
    if m:
        return int(m.group(1), 16)
    bare = re.search(
        rf"- \[(0x{region_start:06x}), asm\]\s*\n", text, re.I
    )
    if bare:
        return int(bare.group(1), 16)
    return None


def remove_empty_tail(text: str, tail_name: str) -> str:
    line = re.search(
        rf"\n      - \[0x[0-9A-Fa-f]+, asm, {re.escape(tail_name)}\]\n", text
    )
    if line:
        return text.replace(line.group(0), "\n", 1)
    return text


class MonolithJob:
    def __init__(self, args: argparse.Namespace) -> None:
        self.monolith = ROOT / args.asm
        self.tail_name = args.tail_name
        self.prefix = args.prefix
        self.region_start = args.region_start
        self.region_end = args.region_end
        self.yaml_subpath = args.yaml_subpath
        self.configure_anchor = args.configure_anchor
        self.ps2_dir = Path(args.ps2_dir)

    def segment_name(self, glabel: str) -> str:
        return f"{self.prefix}{glabel}"

    def yaml_path(self, seg: str) -> str:
        return f"{self.yaml_subpath}/{seg}"

    def tail_asm_path(self) -> Path:
        tail = ROOT / "asm" / f"{self.tail_name}.s"
        if tail.is_file():
            return tail
        return self.monolith

    def apply_yaml(self, batch: list[tuple[str, int, int]], tail_yaml: int, dry_run: bool) -> None:
        text = YAML_PATH.read_text(encoding="utf-8")
        if tail_yaml == self.region_start:
            anchor_re = re.compile(
                rf"      - \[0x{self.region_start:06x}, asm\]\s*\n", re.I
            )
            m = anchor_re.search(text)
            if not m:
                raise SystemExit(f"yaml anchor missing at 0x{self.region_start:06x}")
            anchor = m.group(0)
        else:
            anchor = f"      - [0x{tail_yaml:06X}, asm, {self.tail_name}]\n"
        if anchor not in text:
            raise SystemExit(f"yaml anchor missing: {anchor.strip()}")

        lines: list[str] = []
        for glabel, _size, rom in batch:
            seg = self.segment_name(glabel)
            lines.append(f"      - [0x{rom:06X}, asm, {self.yaml_path(seg)}]\n")
        new_end = batch[-1][2] + batch[-1][1]
        lines.append(f"      - [0x{new_end:06X}, asm, {self.tail_name}]\n")
        replacement = "".join(lines)

        if dry_run:
            print("--- yaml ---")
            print(replacement[:600])
            return
        YAML_PATH.write_text(text.replace(anchor, replacement, 1))
        print(f"yaml: {len(batch)} segments, tail now at 0x{new_end:06X}")

    def configure_ps2_line(self, seg: str) -> str:
        ps2_rel = self.ps2_dir.as_posix()
        if seg.startswith(self.prefix):
            return f'                "{seg}": Path("{ps2_rel}/{seg}_ps2.s"),\n'
        if seg.startswith("early_"):
            return f'                "{seg}": Path("src/early/{seg}_ps2.s"),\n'
        if seg.startswith("rom21_"):
            return f'                "{seg}": Path("src/mem/{seg}_ps2.s"),\n'
        if seg.startswith("heapgap_"):
            return f'                "{seg}": Path("src/heapgap/{seg}_ps2.s"),\n'
        if seg.startswith("bx218_"):
            return f'                "{seg}": Path("src/bx218/{seg}_ps2.s"),\n'
        return f'                "{seg}": Path("src/visualfx/{seg}_ps2.s"),\n'

    def configure_anchor_seg(self, batch: list[tuple[str, int, int]]) -> str:
        text = YAML_PATH.read_text(encoding="utf-8")
        seg0 = self.segment_name(batch[0][0])
        for i, line in enumerate(text.splitlines()):
            if seg0 in line and f"0x{batch[0][2]:06X}" in line.lower():
                if i > 0:
                    m = re.search(
                        rf"{re.escape(self.yaml_subpath)}/({self.prefix}[A-Za-z0-9_]+)",
                        text.splitlines()[i - 1],
                    )
                    if m:
                        return m.group(1)
                break
        return self.configure_anchor

    def apply_configure(self, batch: list[tuple[str, int, int]], dry_run: bool) -> None:
        cfg = CONFIGURE_PATH.read_text(encoding="utf-8")
        anchor_seg = self.configure_anchor_seg(batch)
        anchor = self.configure_ps2_line(anchor_seg)
        if anchor not in cfg:
            raise SystemExit(f"configure anchor missing: {anchor_seg}")

        insert = []
        for glabel, _size, _rom in batch:
            seg = self.segment_name(glabel)
            insert.append(self.configure_ps2_line(seg))

        if dry_run:
            print("--- configure ---")
            print("".join(insert[:3]))
            return
        if f'"{self.segment_name(batch[0][0])}"' in cfg:
            print("configure: batch already present")
            return
        CONFIGURE_PATH.write_text(cfg.replace(anchor, anchor + "".join(insert), 1))
        print(f"configure: +{len(batch)} entries (anchor {anchor_seg})")

    def emit_ps2(self, batch: list[tuple[str, int, int]]) -> int:
        count = 0
        self.ps2_dir.mkdir(parents=True, exist_ok=True)
        for glabel, _size, _rom in batch:
            seg = self.segment_name(glabel)
            asm = ROOT / "asm" / self.yaml_subpath / f"{seg}.s"
            ps2 = self.ps2_dir / f"{seg}_ps2.s"
            if not asm.is_file():
                raise SystemExit(f"missing splat asm: {asm}")
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
            count += 1
        return count

    def verify_objects(self, batch: list[tuple[str, int, int]]) -> tuple[int, int]:
        matched = 0
        for glabel, _size, _rom in batch:
            seg = self.segment_name(glabel)
            target = ROOT / "obj/target" / f"{seg}.o"
            current = ROOT / "obj/current" / f"{seg}.o"
            if target.is_file() and current.is_file():
                if subprocess.run(["cmp", "-s", str(target), str(current)], check=False).returncode == 0:
                    matched += 1
        return matched, len(batch)

    def run_once(self, batch_size: int, dry_run: bool, yaml_only: bool, skip_build: bool, skip_ps2: bool) -> int:
        funcs = parse_monolith(self.tail_asm_path())
        if not funcs:
            return 0

        text = YAML_PATH.read_text(encoding="utf-8")
        tail_yaml = find_tail_yaml(text, self.tail_name, self.region_start)
        if tail_yaml is None:
            print(f"{self.tail_name}: fully split")
            return 0
        if tail_yaml >= self.region_end:
            YAML_PATH.write_text(remove_empty_tail(text, self.tail_name))
            print(f"removed empty {self.tail_name}")
            return 0

        start_idx = 0
        for i, (_g, _s, rom) in enumerate(funcs):
            if rom == tail_yaml:
                start_idx = i
                break
        else:
            raise SystemExit(f"no function at tail 0x{tail_yaml:06X}")

        batch = funcs[start_idx : start_idx + batch_size]
        if not batch:
            return 0

        print(f"split {len(batch)} @ 0x{tail_yaml:06X}: {batch[0][0]} .. {batch[-1][0]}")

        self.apply_yaml(batch, tail_yaml, dry_run)
        self.apply_configure(batch, dry_run)
        if dry_run or yaml_only:
            return 0

        if not skip_build:
            r = subprocess.run(
                [str(ROOT / "venv/bin/python"), str(ROOT / "configure.py"), "-o"],
                cwd=ROOT,
                check=False,
            )
            if r.returncode:
                return r.returncode

        if not skip_ps2:
            n = self.emit_ps2(batch)
            print(f"ps2: {n} files")
            sync = ROOT / "tools/sync_ps2_stubs.py"
            if sync.is_file():
                subprocess.run(
                    [
                        sys.executable,
                        str(sync),
                        "--prefix",
                        self.prefix,
                        "--ps2-dir",
                        self.ps2_dir.as_posix(),
                        "--asm-subpath",
                        self.yaml_subpath,
                    ],
                    cwd=ROOT,
                    check=False,
                )

        if not skip_build:
            ninja_targets: list[str] = []
            for glabel, _size, _rom in batch:
                seg = self.segment_name(glabel)
                ninja_targets.append(f"obj/target/{seg}.o")
                ninja_targets.append(f"obj/current/{seg}.o")
            if subprocess.run(["ninja", *ninja_targets], cwd=ROOT, check=False).returncode:
                return 1

        matched, total = self.verify_objects(batch)
        print(f"objdiff bytes: {matched}/{total} match")
        print(f"tail remainder: ~{len(funcs) - start_idx - len(batch)} functions")
        return 0 if matched == total else 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--asm", required=True, help="monolith path relative to repo root")
    ap.add_argument("--tail-name", required=True, help="yaml tail segment name")
    ap.add_argument("--prefix", required=True, help="segment prefix, e.g. early_func_")
    ap.add_argument("--region-start", type=lambda x: int(x, 0), required=True)
    ap.add_argument("--region-end", type=lambda x: int(x, 0), required=True)
    ap.add_argument("--yaml-subpath", required=True, help="asm subdir, e.g. early or bx218")
    ap.add_argument("--ps2-dir", default="src/early", help="ps2 output directory")
    ap.add_argument("--configure-anchor", required=True)
    ap.add_argument("--batch", type=int, default=50)
    ap.add_argument("--until-empty", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--yaml-only", action="store_true")
    ap.add_argument("--skip-build", action="store_true")
    ap.add_argument("--skip-ps2", action="store_true")
    args = ap.parse_args()

    job = MonolithJob(args)

    if args.until_empty:
        round_idx = 0
        while find_tail_yaml(YAML_PATH.read_text(encoding="utf-8"), job.tail_name, job.region_start) is not None:
            if not parse_monolith(job.tail_asm_path()):
                break
            round_idx += 1
            print(f"=== {job.tail_name} round {round_idx} ===", flush=True)
            if job.run_once(args.batch, args.dry_run, args.yaml_only, args.skip_build, args.skip_ps2):
                return 1
        print(f"{job.tail_name} fully split")
        return 0

    return job.run_once(args.batch, args.dry_run, args.yaml_only, args.skip_build, args.skip_ps2)


if __name__ == "__main__":
    raise SystemExit(main())
