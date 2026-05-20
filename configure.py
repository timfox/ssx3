#!/usr/bin/env python3
import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Set, Union

import ninja_syntax
import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent.resolve()
TOOLS_DIR = ROOT / "tools"
OUTDIR = "out"

YAML_FILE = ROOT / "config" / "ssx3_us.yaml"
BASENAME = "SLUS_207.72"
LD_PATH = f"{BASENAME}.ld"
ELF_PATH = f"{OUTDIR}/{BASENAME}"
MAP_PATH = f"{OUTDIR}/{BASENAME}.map"
PRE_ELF_PATH = f"{OUTDIR}/{BASENAME}.elf"
UNDEFINED_SYMS_AUTO_PATH = "config/undefined_syms_auto.txt"
NATIVE_PATH = f"{OUTDIR}/ssx3-native"
NATIVE_HOST_SOURCES_BASE = [
    "src/platform/linux/ssx3_native.cpp",
    "src/platform/linux/host_disc.cpp",
    "src/platform/linux/host_smoke_test.cpp",
    "src/platform/linux/host_vulkan_renderer.cpp",
    "src/platform/linux/host_vulkan_context.cpp",
    "src/platform/linux/host_frame_pipeline.cpp",
    "src/platform/linux/host_gfx.cpp",
    "src/platform/linux/host_main_menu.cpp",
    "src/platform/linux/host_vulkan_menu.cpp",
    "src/platform/linux/video/host_ea_container.cpp",
    "src/platform/linux/video/host_video_player.cpp",
    "src/platform/linux/video/host_boot_sequence.cpp",
    "src/hashvalue.cpp",
]
HOST_SHADERS = [
    ("shaders/host/triangle.vert", "obj/host/shaders/triangle.vert.spv"),
    ("shaders/host/triangle.frag", "obj/host/shaders/triangle.frag.spv"),
    ("shaders/host/sprite.vert", "obj/host/shaders/sprite.vert.spv"),
    ("shaders/host/sprite.frag", "obj/host/shaders/sprite.frag.spv"),
    ("shaders/host/upscale.comp", "obj/host/shaders/upscale.comp.spv"),
]


def pkg_config(*args: str) -> str:
    return subprocess.check_output(["pkg-config", *args], text=True).strip()


def host_native_sources() -> list[str]:
    sources = list(NATIVE_HOST_SOURCES_BASE)
    dlss_root = os.environ.get("DLSS_SDK_ROOT", "")
    dlss_path = Path(dlss_root) if dlss_root else None
    dlss_header = dlss_path / "include" / "nvsdk_ngx_vk.h" if dlss_path else None
    dlss_lib = dlss_path / "lib" / "Linux_x86_64" / "libnvsdk_ngx.a" if dlss_path else None
    if dlss_header and dlss_header.exists() and dlss_lib and dlss_lib.exists():
        sources.append("src/platform/linux/host_dlss_upscale.cpp")
    else:
        sources.append("src/platform/linux/host_dlss_stub.cpp")
    return sources


def host_build_flags() -> tuple[str, str]:
    cflags = pkg_config("--cflags", "sdl2", "vulkan")
    libs = pkg_config("--libs", "sdl2", "vulkan")
    cxxflags = (
        f"-std=c++17 -Wall -Wextra -pedantic -O2 -DSSX3_HOST -DSKIP_ASM "
        f"-DSSX3_HAVE_SDL -Iinclude -I. {cflags}"
    )

    try:
        ff_cflags = pkg_config(
            "--cflags", "libavcodec", "libavutil", "libswscale", "libswresample", "libavformat"
        )
        ff_libs = pkg_config(
            "--libs", "libavcodec", "libavutil", "libswscale", "libswresample", "libavformat"
        )
        cxxflags += f" -DSSX3_HAVE_FFMPEG {ff_cflags}"
        libs += f" {ff_libs}"
        print("FFmpeg enabled for host video decode (MPEG-2 / VP6 / Bink)")
    except subprocess.CalledProcessError:
        print("FFmpeg dev packages not found; install libavcodec-dev for boot movies")

    dlss_root = os.environ.get("DLSS_SDK_ROOT", "")
    if dlss_root:
        dlss_path = Path(dlss_root)
        dlss_header = dlss_path / "include" / "nvsdk_ngx_vk.h"
        dlss_lib = dlss_path / "lib" / "Linux_x86_64" / "libnvsdk_ngx.a"
        if dlss_header.exists() and dlss_lib.exists():
            cxxflags += f" -DSSX3_HAVE_DLSS -I{dlss_path / 'include'}"
            libs += f" {dlss_lib} -ldl -lpthread"
            print(f"DLSS SDK enabled: {dlss_path}")
            print(f"  runtime: export LD_LIBRARY_PATH={dlss_path / 'lib/Linux_x86_64/rel'}:$LD_LIBRARY_PATH")
        elif dlss_header.exists():
            print(f"DLSS headers found at {dlss_path} but missing {dlss_lib}; using DLSS stub")
        else:
            print(f"DLSS_SDK_ROOT set but headers not found under {dlss_path}")

    return cxxflags, libs

COMMON_INCLUDES = "-Iinclude -isystem include/sdk/ee -isystem include/gcc"


CC_DIR = f"{TOOLS_DIR}/cc/eegcc-2.95.3-V1.36"
DRIVER_PATH_FLAG = f"-B{CC_DIR}/lib/gcc-lib/ee/2.95.3/"

# See tools/cc/README.md for how these were gathered
COMMON_CFLAGS = "-O2"
COMMON_CXXFLAGS = ""

COMPILE_C_RULE = f"{CC_DIR}/bin/ee-gcc2953.exe -c {COMMON_INCLUDES} {DRIVER_PATH_FLAG} {COMMON_CFLAGS} $in"
COMPILE_CXX_RULE = f"{CC_DIR}/bin/ee-gcc2953.exe -xc++ -c {COMMON_INCLUDES} {DRIVER_PATH_FLAG} {COMMON_CFLAGS} {COMMON_CXXFLAGS} $in"

CATEGORY_MAP = {
    "sce": "Libs",
    "data": "Data",
}

WINE = "wine"
if sys.platform == "linux" or sys.platform == "linux2":
    COMPILE_C_RULE = f"{WINE} {COMPILE_C_RULE}"
    COMPILE_CXX_RULE = f"{WINE} {COMPILE_CXX_RULE}"

def clean():
    files_to_clean = [
        ".splache",
        ".ninja_log",
        "build.ninja",
        "permuter_settings.toml",
        LD_PATH,
        UNDEFINED_SYMS_AUTO_PATH,
    ]
    for filename in files_to_clean:
        if os.path.exists(filename):
            os.remove(filename)

    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("obj", ignore_errors=True)
    shutil.rmtree("out", ignore_errors=True)


def write_permuter_settings():
    with open("permuter_settings.toml", "w", encoding="utf-8") as f:
        f.write(f"""compiler_command = "{COMPILE_CXX_RULE} -D__GNUC__"
assembler_command = "mips-linux-gnu-as -march=r5900 -mabi=32 -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"tools/cc/eegcc-2.95.3-V1.36/bin/gcc" = "eegcc-2.95.3-V1.36"
""")

def build_stuff(linker_entries: List[LinkerEntry], skip_checksum=False, objects_only=False, dual_objects=False):
    """
    Build the objects and the final ELF file.
    If objects_only is True, only build objects and skip linking/checksum.
    If dual_objects is True, build objects twice: once normally, once with -DSKIP_ASM.
    """
    built_objects: Set[Path] = set()
    objdiff_units = []  # For objdiff.json

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = None,
        implicit_outputs: List[str] = None,
        out_dir: str = None,
        extra_flags: str = "",
        collect_objdiff: bool = False,
        orig_entry=None,
    ):
        """
        Helper function to build objects.
        """
        # Handle none parameters
        if variables is None:
            variables = {}

        if implicit_outputs is None:
            implicit_outputs = []

        # Convert object_paths to list if it is not already
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        # Only rewrite output path to .o if out_dir is set (i.e. --objects mode)
        if out_dir:
            new_object_paths = []
            for obj in object_paths:
                obj = Path(obj)
                stem = obj.stem
                if obj.suffix in [".s", ".c" , ".cpp"]:
                    stem = obj.stem
                else:
                    if obj.suffix == ".o" and obj.with_suffix("").suffix in [".cpp",".s", ".c"]:
                        stem = obj.with_suffix("").stem
                target_dir = out_dir if out_dir else obj.parent
                new_obj = Path(target_dir) / (stem + ".o")
                new_object_paths.append(new_obj)
            object_paths = new_object_paths

        # Otherwise, use the original object_paths (with .s.o, .c.o, etc.)

        # Convert paths to strings
        object_strs = [str(obj) for obj in object_paths]

        # Add object paths to built_objects
        for idx, object_path in enumerate(object_paths):
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            # Add extra_flags to variables if present
            build_vars = variables.copy()
            if extra_flags:
                build_vars["cflags"] = extra_flags
            ninja.build(
                outputs=[str(object_path)],
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=build_vars,
                implicit_outputs=implicit_outputs,
            )
            # Collect for objdiff.json if requested
            if collect_objdiff and orig_entry is not None:
                src = src_paths[0] if src_paths else None
                if src:
                    src = Path(src)
                    # Always use the final "matched" name, i.e. as if it will be in src/ with no asm/ prefix
                    try:
                        # If the file is in asm/, replace asm/ with nothing (just drop asm/)
                        if src.parts[0] == "asm":
                            rel = Path(*src.parts[1:])
                        elif src.parts[0] == "src":
                            rel = Path(*src.parts[1:])
                        else:
                            rel = src
                        # Remove extension for the name
                        name = str(rel.with_suffix(""))
                    except Exception:
                        name = str(src.with_suffix(""))
                else:
                    name = object_path.stem
                if "target" in str(object_path):
                    target_path = str(object_path)
                    # Determine if a .c or .cpp file exists in src/ for this unit (recursively)
                    src_base = rel.with_suffix("")
                    src_c_files = list(Path("src").rglob(src_base.name + ".c"))
                    src_cpp_files = list(Path("src").rglob(src_base.name + ".cpp"))
                    has_src = bool(src_c_files or src_cpp_files)
                    # Determine the category based on the name
                    categories = [name.split("/")[0]]
                    unit = {
                        "name": name,
                        "target_path": target_path,
                        "metadata": {
                            "progress_categories": categories,
                        }
                    }
                    if has_src:
                        base_path = str(object_path).replace("target", "current")
                        unit["base_path"] = base_path
                    objdiff_units.append(unit)

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w", encoding="utf-8"), width=9999)

    cross = "mips-linux-gnu-"
    patch_abi = f"python3 {ROOT}/tools/patch_o32_abi.py"
    gen_undef_syms = f"python3 {ROOT}/tools/generate_undefined_syms.py $out"

    ld_args = f"-EL -T {UNDEFINED_SYMS_AUTO_PATH} -T config/undefined_funcs_auto.txt -Map $mapfile -T $in -o $out"

    ninja.rule(
        "as",
        description="as $in",
        command=f"cpp {COMMON_INCLUDES} $in -o - | sed 's/\\.set noat/.set at/' | {cross}as -no-pad-sections -EL -march=5900 -mabi=32 -Iinclude -o $out",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{COMPILE_C_RULE} $cflags -o $out && {patch_abi} $out && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "cpp",
        description="cpp $in",
        command=f"{COMPILE_CXX_RULE} $cflags -o $out && {patch_abi} $out && {cross}strip $out -N dummy-symbol-name",
    )

    ninja.rule(
        "gen_undef_syms",
        description="gen undefined_syms_auto.txt",
        command=gen_undef_syms,
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{cross}ld {ld_args}",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{cross}objcopy $in $out -O binary",
    )

    ninja.rule(
        "host_cc",
        description="host-cc $in",
        command="mkdir -p obj/host && c++ $hostflags -c $in -o $out",
    )

    ninja.rule(
        "spirv",
        description="spirv $in",
        command="mkdir -p obj/host/shaders && glslangValidator -V $in -o $out",
    )

    ninja.rule(
        "host_link",
        description="host-link $out",
        command=f"mkdir -p {OUTDIR} && c++ $hostflags $in -o $out $hostlibs",
    )

    # Add recipes for everything
    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "cc", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "cc", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "cc")
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "cpp", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "cpp", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "cpp")
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.bin.CommonSegBin):
            if dual_objects:
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/target", collect_objdiff=True, orig_entry=entry)
                build(entry.object_path, entry.src_paths, "as", out_dir="obj/current", extra_flags="-DSKIP_ASM")
            else:
                build(entry.object_path, entry.src_paths, "as")
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    if objects_only:
        # Write objdiff.json if dual_objects (i.e. --objects)
        if dual_objects:
            objdiff = {
                "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
                "custom_make": "ninja",
                "custom_args": [],
                "build_target": False,
                "build_base": True,
                "watch_patterns": [
                    "src/**/*.c",
                    "src/**/*.cp",
                    "src/**/*.cpp",
                    "src/**/*.cxx",
                    "src/**/*.h",
                    "src/**/*.hp",
                    "src/**/*.hpp",
                    "src/**/*.hxx",
                    "src/**/*.s",
                    "src/**/*.S",
                    "src/**/*.asm",
                    "src/**/*.inc",
                    "src/**/*.py",
                    "src/**/*.yml",
                    "src/**/*.txt",
                    "src/**/*.json"
                ],
                "units": objdiff_units,
                "progress_categories": [ {"id": id, "name": name} for id, name in CATEGORY_MAP.items() ],
            }
            with open("objdiff.json", "w", encoding="utf-8") as f:
                json.dump(objdiff, f, indent=2)
        return

    host_cxxflags, host_libs = host_build_flags()

    host_shader_outputs = []
    for src, out in HOST_SHADERS:
        host_shader_outputs.append(out)
        ninja.build(out, "spirv", src)

    native_objects = []
    for src in host_native_sources():
        obj = Path("obj/host") / (Path(src).stem + ".o")
        native_objects.append(str(obj))
        ninja.build(
            str(obj),
            "host_cc",
            src,
            variables={"hostflags": host_cxxflags},
        )

    ninja.build(
        NATIVE_PATH,
        "host_link",
        native_objects,
        implicit=host_shader_outputs,
        variables={"hostflags": host_cxxflags, "hostlibs": host_libs},
    )
    ninja.build("native", "phony", NATIVE_PATH)

    if not objects_only and os.path.exists(UNDEFINED_SYMS_AUTO_PATH):
        os.remove(UNDEFINED_SYMS_AUTO_PATH)

    ninja.build(
        UNDEFINED_SYMS_AUTO_PATH,
        "gen_undef_syms",
        implicit=[str(obj) for obj in built_objects] + ["tools/generate_undefined_syms.py"],
    )

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects] + [UNDEFINED_SYMS_AUTO_PATH],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        ELF_PATH,
        "elf",
        PRE_ELF_PATH,
    )

    # FIXME: Do this in a more elegant way;
    # probably a python script which reads the splat yaml to get the SHA1,
    # rather than needing a seperate sha1 file.
    # My obession with DRY will never stop >:3
    if not skip_checksum:
        ninja.build(
            ELF_PATH + ".ok",
            "sha1sum",
            "config/checksum.sha1",
            implicit=[ELF_PATH],
        )
    else:
        print("Not adding sha1 rule")

def main():
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean artifacts and build",
        action="store_true",
    )
    parser.add_argument(
        "-C",
        "--only-clean",
        help="Only clean artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-s",
        "--skip-checksum",
        help="Skip the checksum step",
        action="store_true",
    )
    parser.add_argument(
        "-o",
        "--objects",
        help="Build objects to obj/target and obj/current (with -DSKIP_ASM), skip linking and checksum",
        action="store_true",
    )
    args = parser.parse_args()

    do_clean = (args.clean or args.only_clean) or False

    # FIXME: Set skip_checksum to False once builds are able
    # to match; it has been temporairly enabled here.
    do_skip_checksum = args.skip_checksum or True
    do_objects = args.objects or False

    if do_clean:
        clean()
        if args.only_clean:
            return

    split.main([YAML_FILE], modes="all", verbose=False)

    linker_entries = split.linker_writer.entries

    if do_objects:
        build_stuff(linker_entries, skip_checksum=True, objects_only=True, dual_objects=True)
    else:
        build_stuff(linker_entries, do_skip_checksum)

    write_permuter_settings()

if __name__ == "__main__":
    main()
