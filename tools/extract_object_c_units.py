#!/usr/bin/env python3
"""Emit src/mem/units/<segment>.cpp per SKIP_ASM function in src/object/*.cpp for objdiff."""

from __future__ import annotations

import re
from pathlib import Path

import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from ps2_s_to_retail_asm import fn_declaration, retail_asm_from_ps2  # noqa: E402
MEM_DIR = ROOT / "src/mem"
OBJECT_DIR = ROOT / "src/object"
UNITS_DIR = ROOT / "src/mem/units"
INCLUDE_ASM_RE = re.compile(r'INCLUDE_ASM\("([^"]+)",\s*(\w+)\);')


def segment_key(_asm_folder: str, glabel: str) -> str:
    return f"rom21_{glabel}"


def extract_static_functions(head: str) -> list[str]:
    """Pull complete static helper definitions from the pre-INCLUDE_ASM section."""
    out: list[str] = []
    i = 0
    while True:
        m = re.search(r"\bstatic\s+", head[i:])
        if not m:
            break
        start = i + m.start()
        brace = head.find("{", i + m.end())
        if brace < 0:
            break
        depth = 0
        j = brace
        while j < len(head):
            ch = head[j]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    out.append(head[start : j + 1])
                    i = j + 1
                    break
            j += 1
        else:
            break
    return out


HOST_CPP_MARKERS = (
    "operator_new",
    "static_cast",
    "nullptr",
    "std::",
    "kBucketStride",
    "new (",
    "memset",
    "memcpy",
)


def skip_asm_defines_multiple_functions(body: str) -> bool:
    return len(re.findall(r"\b(?:void\*?|int)\s+\w+\s*\([^)]*\)\s*\{", body)) > 1


def is_host_skip_asm_body(body: str) -> bool:
    if "defined(SKIP_ASM) && defined(SSX3_HOST)" in body:
        return True
    stripped = strip_ssx3_host_for_unit(body)
    return any(marker in stripped for marker in HOST_CPP_MARKERS)


def strip_ssx3_host_for_unit(text: str) -> str:
    """Drop SSX3_HOST-only branches so EE units see retail (#else) code."""
    out: list[str] = []
    i = 0
    while i < len(text):
        m = re.match(r"#ifdef SSX3_HOST\b", text[i:])
        if not m:
            n = text.find("#ifdef SSX3_HOST", i)
            if n < 0:
                out.append(text[i:])
                break
            out.append(text[i:n])
            i = n
            continue
        i += m.end()
        depth = 1
        else_start: int | None = None
        end_start = i
        while depth > 0:
            m2 = re.search(r"#(ifdef|ifndef|if|else|elif|endif)\b", text[i:])
            if not m2:
                out.append(text[i:])
                return "".join(out)
            tag = m2.group(1)
            if depth == 1 and tag == "else":
                else_start = i + m2.end()
            if tag in ("ifdef", "ifndef", "if"):
                depth += 1
            elif tag == "endif":
                depth -= 1
                if depth == 0:
                    end_start = i + m2.start()
            i += m2.end()
        if else_start is not None:
            out.append(text[else_start:end_start])
    return "".join(out)


def normalize_unit_body(body: str) -> str:
    """EE objdiff needs unmangled symbols — strip per-function SSX3_HOST extern C guards."""
    if "PS2_RETAIL_ASM" in body:
        return body
    body = re.sub(
        r"#ifdef SSX3_HOST\s*\nextern \"C\"\s*\n#endif\s*\n",
        "",
        body,
    )
    stripped = body.strip()
    if stripped.startswith("extern \"C\""):
        return body
    return f"extern \"C\" {{\n{body}\n}}\n"


def extract_skip_asm_body(text: str, skip_pos: int) -> str | None:
    """Return body between #ifdef SKIP_ASM and its matching #endif."""
    start = text.find("#ifdef SKIP_ASM", skip_pos)
    if start < 0:
        return None
    i = start + len("#ifdef SKIP_ASM")
    depth = 1
    while depth > 0:
        m = re.search(r"#(ifdef|ifndef|if|else|elif|endif)\b", text[i:])
        if not m:
            return None
        tag = m.group(1)
        i += m.end()
        if tag in ("ifdef", "ifndef", "if"):
            depth += 1
        elif tag == "endif":
            depth -= 1
    return text[start + len("#ifdef SKIP_ASM") : i - len("#endif")]


def append_callee_decls(out: list[str], body: str, text: str, glabel: str | None) -> None:
    blob = "".join(out)
    for callee in sorted(set(re.findall(r"\b(func_[0-9A-F]{8})\s*\(", body))):
        if glabel and callee == glabel:
            continue
        if f"{callee}(" in blob and "extern" in blob.split(callee)[0][-40:]:
            continue
        m = re.search(
            rf"^((?:extern\s+)?(?:void\*?|void)\s+{re.escape(callee)}\s*\([^)]*\)\s*;)\s*$",
            text,
            re.M,
        )
        if m:
            decl = m.group(1)
            if not decl.startswith("extern"):
                decl = f"extern {decl}"
            if decl not in blob:
                out.append(decl + "\n")
                blob += decl
            continue
        m = re.search(
            rf"^(void\*?|void)\s+{re.escape(callee)}\s*(\([^)]*\))\s*\{{",
            text,
            re.M,
        )
        if m:
            decl = f"extern {m.group(1)} {callee}{m.group(2)};"
            if decl not in blob:
                out.append(decl + "\n")
                blob += decl


def dedupe_extern_decls_from_body(header: str, body: str) -> str:
    lines: list[str] = []
    for line in body.splitlines():
        stripped = line.strip()
        if stripped.startswith("extern ") and stripped.rstrip(";") + ";" in header:
            continue
        if stripped.startswith("extern ") and stripped in header:
            continue
        lines.append(line)
    return "\n".join(lines) + ("\n" if body.endswith("\n") else "")


def append_file_extern_decls(out: list[str], text: str, glabel: str | None) -> None:
    blob = "".join(out)
    for m in re.finditer(
        r"^((?:extern\s+)?(?:void\*?|int)\s+\w+\s*\([^)]*\)\s*;)\s*$",
        text,
        re.M,
    ):
        decl = m.group(1)
        sym_m = re.search(r"\s(\w+)\s*\(", decl)
        if not sym_m:
            continue
        sym = sym_m.group(1)
        if glabel and sym == glabel:
            continue
        if sym.startswith("D_"):
            continue
        if not decl.startswith("extern"):
            decl = f"extern {decl}"
        if decl not in blob:
            out.append(decl + "\n")
            blob += decl


def parent_header(text: str, full_text: str, glabel: str | None = None) -> str:
    """Shared includes, externs, and static helpers (no SKIP_ASM bodies)."""
    cut = text.find("INCLUDE_ASM")
    head = strip_ssx3_host_for_unit(text[:cut] if cut >= 0 else text)
    if glabel:
        for m in re.finditer(
            rf"(extern\s+)?(void|void\*|int)\s+{re.escape(glabel)}\s*\([^;]*\)\s*;",
            text,
        ):
            if m.group(0) not in head:
                head += m.group(0) + "\n"
        head = re.sub(
            rf"^\s*(extern\s+)?(void|void\*|int)\s+{re.escape(glabel)}\s*\([^;]*\)\s*;\s*\n",
            "",
            head,
            flags=re.M,
        )
    decl_scan = head
    lines: list[str] = []
    for line in decl_scan.splitlines():
        s = line.strip()
        if glabel and (s.startswith(f"void {glabel}(") or s.startswith(f"void* {glabel}(")):
            continue
        if s.startswith("#include") and "<cstdint>" not in s:
            lines.append(line)
        elif s.startswith("typedef") or s.startswith("extern "):
            lines.append(line)
        elif s.startswith('extern "C"'):
            lines.append(line)
        elif (s.startswith("extern int D_") or s.startswith("extern char D_")) or (
            s.startswith("extern unsigned char") and s.endswith(";")
        ):
            lines.append(line)
    out = ['#include "common.h"\n']
    if "PS2_RETAIL_ASM" in full_text:
        out.append('#include "ps2_match.h"\n')
    out.append("\n")
    for line in lines:
        if line.strip().startswith("static "):
            continue
        if line.strip().startswith("#include"):
            continue
        out.append(line + "\n")
    # EE objdiff builds use SKIP_ASM without SSX3_HOST — declare globals referenced via &sym.
    if "D_2898" in full_text and "extern int D_2898" not in "".join(out):
        out.append("extern int D_2898;\n")
    if "D_004FF120" in head and "extern unsigned char D_004FF120" not in "".join(out):
        out.append("extern unsigned char D_004FF120[0x50];\n")
    if "D_0048E808" in head and "extern char D_0048E808" not in "".join(out):
        out.append("extern char D_0048E808[];\n")
    if "D_0049E9E8" in full_text and "D_0049E9E8" not in "".join(out):
        out.append("extern char D_0049E9E8[];\n")
    append_file_extern_decls(out, full_text, glabel)
    for sym in (
        "D_0049F250",
        "D_0049F120",
        "D_0049E978",
        "D_0049E9A8",
        "D_0049E990",
        "D_00491C80",
        "D_00491F00",
        "D_00491E80",
        "D_004906F0",
        "D_0044AFF0",
    ):
        if sym in full_text and sym not in "".join(out):
            if sym.startswith("D_0049E") and sym != "D_0049E990":
                out.append(f"extern char {sym}[];\n")
            else:
                out.append(f"extern int {sym};\n")
    return "".join(out)


def main() -> int:
    UNITS_DIR.mkdir(parents=True, exist_ok=True)
    written = 0
    for cpp in sorted(OBJECT_DIR.glob("*.cpp")):
        text = cpp.read_text(encoding="utf-8", errors="replace")
        if "#ifdef SKIP_ASM" not in text:
            continue
        pos = 0
        for m in INCLUDE_ASM_RE.finditer(text):
            asm_folder, glabel = m.group(1), m.group(2)
            body = extract_skip_asm_body(text, m.end())
            if body is None:
                continue
            stripped = re.sub(r"//.*?$|/\*.*?\*/", "", body, flags=re.M | re.S).strip()
            if len(stripped) < 40:
                continue
            key = segment_key(asm_folder, glabel)
            ps2_path = MEM_DIR / f"{key}_ps2.s"
            host_only = "defined(SKIP_ASM) && defined(SSX3_HOST)" in body
            pre_asm = strip_ssx3_host_for_unit(
                text[: text.find("INCLUDE_ASM")] if "INCLUDE_ASM" in text else text
            )
            has_decomp_c = (
                "PS2_RETAIL_ASM" not in body
                and "PS2_RETAIL_ASM_ONLY" not in body
                and not host_only
                and not is_host_skip_asm_body(body)
                and not is_host_skip_asm_body(pre_asm)
                and not skip_asm_defines_multiple_functions(body)
                and re.search(
                    rf"(void|void\*|int|unsigned)\s+{re.escape(glabel)}\s*\(",
                    body,
                )
                is not None
            )
            use_ps2 = ps2_path.is_file() and not has_decomp_c
            use_c = use_ps2 or "PS2_RETAIL_ASM" in body or has_decomp_c
            use_cpp = has_decomp_c and not use_ps2
            out = UNITS_DIR / f"{key}.{'cpp' if use_cpp else ('c' if use_c else 'cpp')}"
            header = parent_header(text, text, glabel)
            if use_c:
                header = re.sub(r"#ifdef SSX3_HOST.*?#endif\n\n?", "", header, flags=re.S)
                header = re.sub(r'#include "ps2_match.h"\n', "", header)
                if "ps2_match.h" not in header:
                    header = header.replace(
                        '#include "common.h"\n',
                        '#include "common.h"\n#include "ps2_match.h"\n',
                    )
            parts = [header]
            if use_ps2:
                decl = fn_declaration(text, glabel)
                _, asm_body, _ = retail_asm_from_ps2(ps2_path, text, glabel)
                unit_body = ""
                if decl:
                    unit_body += f"{decl}\n"
                unit_body += asm_body
                parts.append(f"#ifdef SKIP_ASM\n{unit_body}\n#endif\n")
            else:
                for static_fn in extract_static_functions(pre_asm):
                    if static_fn not in "".join(parts):
                        parts.append(static_fn + "\n\n")
                stripped_body = strip_ssx3_host_for_unit(body)
                callee_decls: list[str] = []
                append_callee_decls(callee_decls, stripped_body, text, glabel)
                if callee_decls:
                    parts[0] = header + "".join(callee_decls)
                unit_body = normalize_unit_body(
                    dedupe_extern_decls_from_body(parts[0], stripped_body)
                )
                if out.suffix == ".c":
                    unit_body = re.sub(
                        r'#ifdef SSX3_HOST\s*\nextern \"C\"\s*\n#endif\s*\n',
                        "",
                        unit_body,
                    )
                    unit_body = re.sub(
                        r"extern \"C\"\s*\{\s*\n",
                        "",
                        unit_body,
                    )
                    unit_body = re.sub(r"\n\}\s*\n?\s*$", "\n", unit_body)
                parts.append(f"#ifdef SKIP_ASM\n{unit_body}#endif\n")
            new_text = strip_ssx3_host_for_unit("".join(parts))
            if out.is_file() and "// @objdiff-matched" in out.read_text(
                encoding="utf-8", errors="replace"
            ):
                new_text = new_text.replace(
                    '#include "common.h"\n',
                    '#include "common.h"\n// @objdiff-matched\n',
                    1,
                )
            out.write_text(new_text, encoding="utf-8")
            for ext in (".c", ".cpp"):
                stale = UNITS_DIR / f"{key}{ext}"
                if stale.is_file() and stale != out:
                    stale.unlink()
            written += 1
            pos = m.end()
    print(f"[object-units] wrote {written} unit(s) under src/mem/units/")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
