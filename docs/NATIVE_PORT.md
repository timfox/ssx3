# Native Linux port architecture

Goal: run **decompiled SSX3 game code** on Linux, with **Vulkan** for presentation — not
long-term reimplementations of menus, video, or gameplay in `src/platform/linux/`.

The PS2 build (`ninja`) remains the source of truth for matching. The native build
(`ninja native`) is a second target that links the same (eventually) C/C++ and replaces
PS2-only services with a **host abstraction layer (HAL)**.

## Current reality (honest)

| Area | Status |
|------|--------|
| Executable size | ~3.8 MB main segment, mostly **disassembly** |
| Named symbols | ~750 in `config/symbol_addrs.txt` |
| C++ with real bodies | **6** files (`hashvalue`, `bxrandom`, `bxstring`, `md5`, `crowdrender2d`, `dirtysock/tags`) |
| Placeholder `src/**/*.cpp` | **~365** files (`//Known file in project` only) |
| Linked into `ssx3-native` today | Host runtime + **`hashvalue.cpp`**, **`bxrandom.cpp`** (decomp) |
| Title screen / FMV | **Host reimplementation** (assets + Vulkan), not `cFEStateTitle` |

So “translate everything” is the right end state, but **most functions are not translated yet** —
they are still MIPS in `asm/nonmatchings/` (generated at PS2 build time). Native work is
**port infrastructure + decompilation**, in parallel.

## Layered model

```
┌─────────────────────────────────────────────────────────┐
│  Decompiled game logic (FE, world, AI, render state, …)   │
│  Built with -DSSX3_HOST -DSKIP_ASM (no INCLUDE_ASM)       │
└───────────────────────────┬─────────────────────────────┘
                            │ calls
┌───────────────────────────▼─────────────────────────────┐
│  Host HAL (implementations of PS2/EA services)          │
│  memory, threads, files, pad, audio, timers, BIG, …     │
└───────────────────────────┬─────────────────────────────┘
                            │ uses
┌───────────────────────────▼─────────────────────────────┐
│  Vulkan presentation (SDL window, swapchain, shaders)   │
│  Fed by graphics HAL — not a second parallel game UI    │
└─────────────────────────────────────────────────────────┘
```

Game code should not call SDL or Vulkan directly. It should call the same
`cMemMan_*`, `cGraphicsMan_*`, `cPSPGraphicsMan_*`, etc. names; those compile to
host-backed implementations on Linux.

## What PS2 services must be replaced

Rough dependency order for reaching the title screen (`cFEStateTitle` @ `0x194778`):

1. **CRT / init** — `main`, `systemInit`, static constructors (`__sti__all_in_one_*`)
2. **Memory** — `cMemMan_alloc` / `free`
3. **Threads / time** — `SYNCTASK_*`, `THREAD_yieldticks`, `cAppMan_mainLoop`
4. **Disc / archives** — `FILESYS_*`, `BIG_*`, `host::disc()` path mapping
5. **Input** — `cInputMap_*`, pad polling (SDL gamepad)
6. **Graphics** — `cPSPGraphicsMan_*`, `cRenderStateMan_*`, `cBackgroundMan_*`,
   `cVisualEffectsMainMenu_*`, DMA/GS path or **deliberate rewrite** to record draws for Vulkan
7. **Audio** — `ps2soundman` path (can stub initially)
8. **FE** — `cFEStateTitle_onCreateScreen`, widget/UI engine

The in-tree Vulkan menu (`host_main_menu.cpp`) is a **stand-in** until (6)+(8) run for real.
It should shrink over time, not grow.

## Graphics: two viable strategies

### A. GS / DMA capture (hardware-style)

Implement enough EE→GS packet handling to interpret what the game pushes (similar in spirit
to PCSX2’s GS path). Highest fidelity to asm, very large effort.

### B. HAL rewrite (recommended for this project)

Decompile `cPSPGraphicsMan`, `cRenderStateMan`, texture upload (`bxps2tex`), and related code;
replace low-level GS/DMA sends with a **Vulkan backend** that keeps EA’s scene graph and state
(`cVisualEffectsMainMenu`, snowflake colours, etc.).

The existing `host_vulkan_renderer.cpp` is a starting point for **presentation**, not the final
API. Expect something like `host_gs_submit(draw_command)` called from ported graphics code.

## Build system direction

Today:

- `configure.py` → `host_native_sources()` lists only host files + `hashvalue.cpp`.
- Game placeholders are **not** compiled for native.

Target:

1. Add `host_native_objects()` entries as files gain `SKIP_ASM` bodies (or dedicated
   `src/platform/stubs/*.cpp` that satisfy linker symbols).
2. Generate `config/undefined_syms_host.txt` from link failures (like PS2
   `undefined_syms_auto.txt`).
3. Optional: compile some asm via **lightweight MIPS interpreter** — usually not worth it
   for a full game; prefer decomp + HAL stubs.

## Phased roadmap

| Phase | Outcome | Depends on |
|-------|---------|------------|
| **0** (now) | Disc check, FMV, hash smoke test, placeholder Vulkan title | — |
| **1** (started) | HAL: `cMemMan_*`, `systemInit`, FILESYS/BIG, `--boot-game` | `host_mem.cpp`, `host_boot.cpp`, `host_big.cpp` |
| **2** (started) | Retail `main()` chain → `cAppMan_mainLoop` host tick loop | `--boot-game` |
| **2b** (started) | Link decomp units (`bxrandom`, …) with smoke tests | `bxrandom.cpp`, `host_game_bss.cpp` |
| **3** (started) | BIGF reads from `disc/` (`BIG_locateentryz`, …) | `host_disc` + `host_big.cpp` |
| **4** | Pad input reaches `cInputMap` | SDL |
| **5** | Graphics HAL draws one FE texture / full-screen quad via Vulkan | Renderer + decomp graphics |
| **6** | **Real `cFEStateTitle`** — retail title layout from game code | Phase 5 + FE decomp |
| **7** | Main menu → mountain room → race | World, streaming, physics, audio |

Phases 1–4 are months of work; 5–7 are multi-year alongside decompilation (same order of
magnitude as other PS2 decomp → PC efforts).

## Phase 1–2 scaffold (in tree)

```bash
ninja native
./out/ssx3-native --boot-game
```

This runs a **C reimplementation of retail `main()`’s call order** (not MIPS `main` yet):

1. `retail_preinit` (stand-in for `0x40fc90`)
2. `systemInit` → `cMemMan_initialize`
3. `cMemMan_alloc(0x142c0, …)` — same size as disc ELF
4. `retail_run_static_init` (stand-in for `0x31af50`, 49 ctor slots)
5. module init loop (stand-in for `0x31b098`)
6. `cExecutionMan_halt(ctx)`
7. `cAppMan_run` → `cAppMan_mainLoop` (SDL window or headless ticks)

Symbols: `include/platform/host_abi.h`, `include/platform/host_syscalls.h`, `src/platform/linux/host_*.cpp`.

## What to do next (concrete)

1. **Keep decomping** using `symbol_addrs.txt` / objdiff — placeholders are not executable code.
2. **Extend `host_native_sources()`** as each module compiles under `SKIP_ASM`.
3. **Replace host boot C chain** with linked decomp of `main` when `main.cpp` exists.
3. **Introduce `include/platform/host_syscalls.h`** (or per-domain headers) documenting which
   PS2 symbols the HAL provides.
4. **Replace host title menu** with phase-6 milestone: call into ported `cFEStateTitle` once
   graphics HAL can present UI.
5. **Do not** duplicate more game systems in `host_*` unless they are temporary stubs with
   a clear deletion ticket when the real symbol is ported.

## References in this repo

- Symbols: `config/symbol_addrs.txt` (`cFEStateTitle`, `cVisualEffectsMainMenu_*`, …)
- Splat layout: `config/ssx3_us.yaml` (only six `cpp` regions today)
- Host entry: `src/platform/linux/ssx3_native.cpp`
- Workflow: `Main Workflow.txt` (`SKIP_ASM`, `INCLUDE_ASM`)
