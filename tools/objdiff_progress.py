#!/usr/bin/env python3
"""Report objdiff .text match rate by region (target vs obj/current)."""

from __future__ import annotations

import argparse
import subprocess
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OBJCOPY = "mips-linux-gnu-objcopy"


def text_bytes(path: Path) -> bytes | None:
    if not path.is_file():
        return None
    out = Path(tempfile.mkstemp(suffix=".text")[1])
    try:
        r = subprocess.run(
            [OBJCOPY, "-O", "binary", "--only-section=.text", str(path), str(out)],
            capture_output=True,
        )
        if r.returncode != 0:
            return None
        return out.read_bytes()
    finally:
        out.unlink(missing_ok=True)


def region_for(name: str) -> str:
    if name.startswith("early/"):
        return "early"
    if name.startswith("late/"):
        return "late"
    if name.startswith("dcd10/"):
        return "dcd10"
    if name.startswith("mem/") or name.startswith("rom21_"):
        return "mem"
    if name.startswith("visualfx/"):
        return "visualfx"
    return name.split("/")[0] if "/" in name else "other"


def check_unit(stem: str) -> tuple[str, str, bool | None]:
    target = ROOT / f"obj/target/{stem}.o"
    current = ROOT / f"obj/current/{stem}.o"
    if not target.is_file():
        return stem, region_for(stem), None
    if not current.is_file():
        return stem, region_for(stem), False
    tt = text_bytes(target)
    cc = text_bytes(current)
    if tt is None or cc is None:
        return stem, region_for(stem), False
    return stem, region_for(stem), tt == cc


def tagged_c_units() -> int:
    units = ROOT / "src/mem/units"
    if not units.is_dir():
        return 0
    n = 0
    for p in list(units.glob("*.c")) + list(units.glob("*.cpp")):
        if "// @objdiff-matched" in p.read_text(encoding="utf-8", errors="replace"):
            n += 1
    return n


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--sample", type=int, default=0, help="Random sample size (0 = all)")
    ap.add_argument("--jobs", type=int, default=8)
    args = ap.parse_args()

    target_dir = ROOT / "obj/target"
    if not target_dir.is_dir():
        print("[objdiff] run: venv/bin/python configure.py -o && ninja")
        return 1

    stems = sorted(p.stem for p in target_dir.glob("*.o"))
    if args.sample and args.sample < len(stems):
        import random

        random.seed(0)
        stems = random.sample(stems, args.sample)

    stats: dict[str, dict[str, int]] = {}
    matched_c = tagged_c_units()

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futs = [pool.submit(check_unit, s) for s in stems]
        for fut in as_completed(futs):
            _stem, reg, ok = fut.result()
            bucket = stats.setdefault(reg, {"match": 0, "diff": 0, "missing": 0})
            if ok is None:
                bucket["missing"] += 1
            elif ok:
                bucket["match"] += 1
            else:
                bucket["diff"] += 1

    total_m = sum(b["match"] for b in stats.values())
    total_d = sum(b["diff"] for b in stats.values())
    total = total_m + total_d
    pct = (100 * total_m // total) if total else 0

    print("SSX3 objdiff .text progress (target vs obj/current)")
    print()
    print(f"  Units checked:     {total}")
    print(f"  .text matched:     {total_m}/{total} ({pct}%)")
    print(f"  .text differ:      {total_d}")
    print(f"  C units (@tag):    {matched_c} (readable/decomp TUs)")
    print()
    print("## By region")
    for reg in sorted(stats.keys()):
        b = stats[reg]
        n = b["match"] + b["diff"]
        p = (100 * b["match"] // n) if n else 0
        print(f"  {reg:12} {b['match']:5}/{n:5}  ({p:3}%)  diff={b['diff']}")
    print()
    print("Note: Most units still use identical *_ps2.s on both sides (100% match).")
    print("      @objdiff-matched C units are the decompiled subset.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
