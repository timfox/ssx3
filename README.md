# SSX 3 (2003)

This is a work-in-progress decompilation of SSX 3 (2003) for the PlayStation 2. This project builds the NTSC-U version of the game (`SLUS_207.72`, sha1 `77114dfd1205eaccf1ccc18c5f9650097fa78bd8`). Other regions and versions may be considered once NTSC-U has been completed and matched.

The main goal of this project is to decompile the game code to matching source code. This project doesn't contain any game assets or original code from the game's executable. It requires your own copy of the game to build and run this project.

**Do not commit game content.** The `disc/` tree, `assets/` extracts, `.iso` files,
`.mpc` / `.big` archives, and similar files stay on your machine only (see
`disc/README.md`, `assets/README.md`, and `scripts/check_no_game_assets.sh`).

## Setup

You will need:

- a Linux system that can run 32 bit applications. WSL2 should work as well.
- a copy of binutils for the `mips-linux-gnu` target
- A NTSC-U disc image (obtained legally)

### Debian/Ubuntu

`# apt install -y binutils-mips-linux-gnu ninja-build wine`  

Download required tools:  

```bash  
# ./scripts/setup.sh
```

Make a `disc` directory in the root of this repository (gitignored), and copy the
files from the NTSC-U game disc. Only source code and tooling belong in git.

Setup a venv for splat:

```bash
$ python -m venv venv  
$ . venv/bin/activate  
(venv) $ pip install -r requirements.txt  
```

`./configure.py` ultimately does the setup.

## Building

`ninja` should do the trick.

## Native Linux port (in progress)

**Direction:** decompile SSX 3 to matching C, then **link that same C** on Linux with
`-DSSX3_HOST -DSKIP_ASM` behind a thin HAL — not a permanent rewrite in
`src/platform/linux/`. Start here: **`docs/NATIVE_DECOMP.md`**.

```bash
python3 tools/native_status.py
python3 scripts/sync_native_decomp.py
venv/bin/python configure.py && ninja native
```

See also `docs/NATIVE_PORT.md` (HAL detail) and `docs/PLAYABLE_ROADMAP.md` (milestones).

This repository does not yet run SSX 3 as a playable native Linux game. Most
game code is still PS2 assembly or placeholder files. The native target is an
incremental host runtime that:

- Validates the extracted disc layout (`SYSTEM.CNF`, boot ELF, pads, netgui)
- Provides host disc I/O (`host::disc()` under `include/platform/`)
- Links decompiled C units compiled with `-DSSX3_HOST -DSKIP_ASM` (starting with
  `hashvalue.cpp`)

Host dependencies (Debian/Ubuntu):

```bash
# apt install -y libsdl2-dev libvulkan-dev glslang-tools \
#   libavcodec-dev libavutil-dev libswscale-dev libavformat-dev
```

Boot FMV (EA BIG + intro) uses in-tree EA chunk demuxers and optional LGPL
FFmpeg decode — no RAD Bink or On2 VP6 SDKs. See `docs/VIDEO_CODECS.md`.
Movies must be non-zero files from a full disc rip (`disc/data/movies/*.mpc`).

Build and run:

```bash
$ . venv/bin/activate
(venv) $ python configure.py
(venv) $ ninja native
(venv) $ out/ssx3-native
```

With `--gfx`, boot movies and the main menu share one window at `--width` x
`--height` (default **1280×720**). Use **`--resolution`** for presets (`720p`,
`1080p`, `1440p`, `4k`, `ps2` for 640×448, etc.); run **`--list-resolutions`**
for the full table. **`--width`** / **`--height`** override a preset. The FE
layout letterboxes to NTSC 640×448 inside any window size; resize the window
live and the swapchain follows. **Space** or **Enter** skips the current movie;
**Escape** quits. **F11** toggles fullscreen (desktop mode). Use `--no-boot-videos` to skip FMV entirely.

After boot FMV (or with `--no-boot-videos`), a **temporary** Vulkan title screen
stands in until real `cFEStateTitle` code runs through the graphics HAL
(see `docs/NATIVE_PORT.md`). Today it uses extracted `fe_1.ssh` art in 640×448
layout; the window is resizable with letterboxing.

Menu text uses a bitmap font atlas. **Retail** (default): `disc/data/fonts/fefont.sfn` → `fe_font_atlas.png` via `scripts/extract_menu_font.py` ([EA-Font-Manager](https://github.com/bartlomiejduda/EA-Font-Manager)). Strings like `press <start> to play` map `<start>` to the retail Start-button glyph.

**Custom** (optional): bake your own TTF into `host_font_atlas.png` + `host_font_metrics.bin`:

```bash
python3 scripts/bake_host_font.py --font /path/to/YourFont.ttf --size 24
```

`--font auto` (default) uses the retail `fefont.sfn` atlas when present; `--font custom` uses `host_font_*` from `bake_host_font.py`. Force retail with `--font retail` on `ssx3-native --gfx`.

Extract all host assets from a disc rip in one pass:

```bash
python3 scripts/extract_host_assets.py --disc disc
```

See `docs/DISC_EXTRACT.md` for per-script detail. Locale strings come from
`data/locale/cmnamer.loc` + `feamer.loc` → `assets/host/locale/fe_strings.json`.

Menu audio WAVs are under `assets/host/audio/`, menu textures in `assets/host/menu/`,
and additional FE SSH tags in `assets/host/ui/`.

Menu audio is decoded from `disc/data/audio/audio.big` on first run
(`scripts/extract_menu_audio.py` uses bundled `tools/vgmstream/vgmstream-cli`):
title wind loop (`BC_AmbLoop1.bnk`), menu music loop (`SSX3Menu.bnk` subsong 1),
short interactive stems (subsongs 4–6, one-shots on D-pad), and UI SFX (`FE Menu.bnk`).
WAVs land in `assets/host/audio/`; **OpenAL** plays wind on the title screen and the
menu loop on other FE screens during `--gfx`.

Vulkan renderer with optional upscaling:

```bash
(venv) $ out/ssx3-native --gfx
(venv) $ out/ssx3-native --gfx --upscale compute --render-scale 0.5
(venv) $ out/ssx3-native --gfx --upscale dlss --render-scale 0.67
```

- **compute** — Vulkan compute shader upscale (always available)
- **dlss** — NVIDIA DLSS Super Resolution (RTX GPU + DLSS SDK; falls back to compute)
- **direct** — no upscaling, render at full resolution

DLSS build setup:

```bash
git clone https://github.com/NVIDIA/DLSS
export DLSS_SDK_ROOT=$PWD/DLSS
python configure.py && ninja native
export LD_LIBRARY_PATH=$DLSS_SDK_ROOT/lib/Linux_x86_64/rel:$LD_LIBRARY_PATH
out/ssx3-native --gfx --upscale dlss
```

Options: `--resolution`, `--width`, `--height`, `--no-vsync`, `--render-scale`, optional positional
`disc` and `rebuilt_elf` paths.

To add more decompiled sources to the native binary, append paths to
`NATIVE_HOST_SOURCES` in `configure.py` (they must build with `SKIP_ASM` bodies
or compile as stubs).
