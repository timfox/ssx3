# Playable native SSX 3 — roadmap

Goal: `./out/ssx3-native` runs **decompiled retail C** behind a **host HAL**, not a permanent rewrite in `src/platform/linux/`.

## Current state (May 2026)

| Layer | Status |
|-------|--------|
| Splat / per-function segments | **Done** (~15k units: early, mem, dcd10, late) |
| objdiff reference (`obj/target`) | **Done** (retail `*_ps2.s` per segment) |
| Matching C (`obj/current`) | **376** `@objdiff-matched` units; **168** readable EE C (jr-delay `void f(void){}` + `func_0034FC78` back-link); `tools/auto_try_jr_delay_units.py` |
| Native binary | **Builds** — **53** decomp TUs; smoke, `--gfx` FE, `--boot-game` (Enter → load track), `--game` flyover |
| World (native) | **HAL state** — `HostWorld` / `HostWorldView`; `cGame_load` → `host_world_create` → `cWorld_resetMap` |
| Playable race | **Started** — `--boot-then-game`: retail load + `cRider_initOnce` + Vulkan terrain per `map_id`; `cSSXApp_preUpdate` ticks race |

## Three parallel tracks

### 1. Map everything (objdiff)

Every function has `obj/target/*.o` (retail) vs `obj/current/*.o` (decomp).

```bash
venv/bin/python configure.py -o && ninja
python3 tools/objdiff_progress.py          # global .text stats
python3 tools/batch_verify_units.py        # tag matching C units
```

Most units already **match** because both sides use the same `*_ps2.s`. Progress = growing the set of **@objdiff-matched C units** with real source.

### 2. Decompile (readable C)

Priority order (boot → race):

1. **Runtime** — `SYNCTASK_*`, `cAppMan_*`, `cMemMan_*` (heap); native: `synctask_retail_impl` (add/del/run from disasm), `retail_memset_u8`, `src/dcd10/cAppMan.cpp` (SKIP_ASM → `host_appman_run`)
2. **Boot chain** — `systemInit`, static init, module init (`docs/SOURCE_PORT.md`)
3. **FE** — `cFEState*` (replace host FE compositor over time)
4. **World / render** — `cWorld_*`, `cPSPGraphicsMan_*`, rider/physics

Workflow per function:

```bash
# Small functions: auto-embed retail opcodes, then replace with real C
python3 tools/generate_ps2_units.py --boot
python3 tools/batch_verify_units.py

# Or decomp.me → SKIP_ASM in src/**/*.cpp → verify
python configure.py -o && ninja
```

### 3. Native port (playable)

```bash
python scripts/sync_native_decomp.py   # link TUs with SKIP_ASM bodies
venv/bin/python configure.py && ninja native
./out/ssx3-native disc                  # smoke + optional boot FMV
./out/ssx3-native --gfx disc            # Vulkan FE menu (host)
./out/ssx3-native --boot-game disc      # retail boot tick (HAL)
./out/ssx3-native --game disc           # procedural 3D (placeholder)
```

HAL grows in `src/platform/linux/host_*.cpp`. Game code stays in `src/**` with `-DSSX3_HOST -DSKIP_ASM`.

## Definition of “playable”

Minimum: **main menu → mountain → load → in-race gameplay** using decompiled logic, disc assets, pad input, Vulkan GS, OpenAL audio.

That requires phases 6–8 in `docs/NATIVE_PORT.md` (FE bridge, world streaming, physics/audio) on top of continued decompilation.

## Commands cheat sheet

```bash
python3 tools/decomp_progress.py
python3 tools/generate_ps2_units.py --boot
python3 tools/batch_verify_units.py
venv/bin/python configure.py -o && ninja
python scripts/sync_native_decomp.py && venv/bin/python configure.py && ninja native
```
