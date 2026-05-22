# SSX 3 source port (decomp-driven)

Goal: a **playable Linux build** that runs **decompiled game C++** behind a **host HAL** (memory, disc, pad, Vulkan GS), not a permanent rewrite of game systems in `src/platform/linux/`.

The PS2 ELF (`ninja`) stays the matching reference. The source port (`ninja native`) grows as `src/**/*.cpp` gain real `#ifdef SKIP_ASM` bodies and are linked from `configure.py`.

## Architecture

```
Retail disc  →  host::disc() / BIG / FILESYS
                      ↓
Decompiled game (FE, world, AI, render, bx, …)   [-DSSX3_HOST -DSKIP_ASM]
                      ↓
Host HAL (cMemMan_*, SYNCTASK_*, cBigFile_*, pad, gs → Vulkan)
                      ↓
SDL + Vulkan window
```

Game code must not call SDL/Vulkan directly. It keeps retail symbol names; HAL provides Linux implementations.

## Critical path (boot → race)

| Order | Retail symbols | Status |
|-------|----------------|--------|
| 1 | `systemInit`, `cMemMan_*`, static init | HAL in `host_mem.cpp`, `host_boot.cpp` |
| 2 | `cAppMan_mainLoop`, `SYNCTASK_*` | HAL stubs + smoke tests |
| 3 | `FILESYS_*`, `BIG_*`, `cBigFile_*` | HAL + disc layout |
| 4 | `cInputMap_*`, `cInputPad_*` | HAL + `input.map` |
| 5 | `cPSPGraphicsMan_*`, `cRenderStateMan_*`, GS | Stubs → `host_gs.cpp` (grow with decomp) |
| 6 | `cFEStateTitle_*` … `cFEStateMainMenu_*` | Host FE compositor today; replace with decomp FE |
| 7 | `cWorld_*`, physics, streaming | Stubs; `--game` procedural stand-in |
| 8 | Audio (`ps2soundman`, …) | OpenAL menu path only |

Phases 1–5 are months of work; 6–8 are multi-year alongside decompilation (typical for PS2 decomp → PC).

## Decompilation workflow

1. Pick a symbol in `config/symbol_addrs.txt` or `asm/nonmatchings/<unit>/`.
2. Match in [decomp.me](https://decomp.me) (SSX 3 preset) or locally from `asm/nonmatchings/*.s`.
3. Add C under `#ifdef SKIP_ASM` in the matching `src/**/*.cpp`.
4. `python configure.py -o && ninja` — build `obj/target/*.o` (asm) vs `obj/current/*.o` (C).
5. `tools/objdiff/objdiff-linux-x86_64 -p .` — verify **100%** match.
6. `python scripts/sync_native_decomp.py` — refresh native link list.
7. `ninja native` — link game TU into `ssx3-native`.

## Linked decompiled units (native)

See `config/native_decomp_sources.txt` (generated). Today:

- `hashvalue.cpp`, `md5.cpp`, `bxrandom.cpp`, `bx/bxstring.cpp` (partial)
- `mem/operator_new.cpp`, `mem/cmemman_alloc.cpp` + `mem/cmemman_alloc_ps2.s` (allocator @ `0x317D70`, hand-matched asm for `obj/current`)
- `visualfx/crowdrender2d.cpp`: `cCrowdRender2D_init` (FILE_load + SHAPE_locate); `cCrowdAnim2D_update` via retail-byte `crowd_anim2d_update_ps2.s`
- `mem/cmemman_alloc`: retail `cmemman_alloc_ps2.s` @ `0x317D70` (48 insns, objdiff match)
- `mem/cmemman_alloc_body`: `cmemman_alloc_body_ps2.s` @ `0x319A90` (184 B, objdiff match)
- `mem/operator_new_global`: `operator_new_global_ps2.s` @ `0x317E30` (28 B, thin `cMemMan_alloc` wrapper)
- `mem/operator_delete`: `operator_delete_ps2.s` @ `0x317E50` (72 B, mutex + `func_00319B48`)
- `mem/cmemman_free`: `cmemman_free_ps2.s` @ `0x317E98` (72 B, mutex wrapper)
- `mem/mem_post_free`: `mem_post_free_ps2.s` @ `0x317EE0` (264 B, post-free helper before bx)
- `mem/cmemman_free_body`: `cmemman_free_body_ps2.s` @ `0x319B48` (yaml `0x21AB48`, 144 B)
- `mem/cmemman_internal_resize`: `cmemman_internal_resize_ps2.s` @ `0x319BD8` (yaml `0x21ABD8`, 328 B)
- `bx/bxstring_mid`: splat asm @ `0x318BD8`–`0x21AB48` (bx gap between free body and `0x21A1C0` blob)
- `mem/cmem_blockman_init`: `cmem_blockman_init_ps2.s` @ `0x319D20` (yaml `0x21AD20`, 296 B)
- `mem/cmemman_alloc_small`: `cmemman_alloc_small_ps2.s` @ `0x319E48` (yaml `0x21AE48`, 284 B)
- `mem/cmemman_heap_dispatch`: `cmemman_heap_dispatch_ps2.s` @ `0x319F68` (yaml `0x21AF68`, 284 B; heap size routing)
- `mem/cmemman_heap_balloc_tiny`: `cmemman_heap_balloc_tiny_ps2.s` @ `0x31A088` (yaml `0x21B088`, 168 B)
- `mem/cmemman_heap_balloc_small`: `cmemman_heap_balloc_small_ps2.s` @ `0x31A130` (yaml `0x21B130`, 204 B)

Generate `*_ps2.s` from splat comments: `python3 tools/gen_ps2_from_splat.py asm/mem/foo.s glabel_name src/mem/foo_ps2.s [--words N]`

All listed mem units: `obj/target/*.o` vs `obj/current/*.o` `.text` sections match byte-for-byte (verified May 2026).

Splat yaml offsets for the main segment use **symbol VA − 0xFF000** (not − 0x100000). Example: `cMemMan_internalResizeBlock` @ `0x319BD8` → yaml `0x21ABD8`.
- `visualfx/crowd_anim2d_update`: separate splat asm segment @ `0x1DCAC0`; retail `.word` asm in `crowd_anim2d_update_ps2.s` (592 B)
- Crowd 2D @ `0x1DC6D8`–`0x1DCD10`: eight retail asm segments (`crowd_head`, `crowdrender2d_init`, `crowd_purge`, `crowd_construct`, `crowd_anim_ctor`, `crowd_advance`, `crowd_anim2d_update`) — 1592 B total, all objdiff-matched via `*_ps2.s`. Host logic in `crowdrender2d.cpp` + `crowdrender2d_init.cpp`.
- `visualfx/crowdrender2d.cpp` (partial — `cCrowdAnim2D_update` in progress)
- `dirtysock/tags.cpp`

## Commands

```bash
# PS2 matching build (reference)
. venv/bin/activate && python configure.py && ninja

# Dual objects for objdiff
python configure.py -o && ninja

# Source port binary
python scripts/sync_native_decomp.py
ninja native
./out/ssx3-native --gfx --font retail --no-boot-videos disc
./out/ssx3-native --boot-then-gfx disc
./out/ssx3-native --game disc
```

## What not to do

- Do not add large new game features in `host_*` without a plan to delete them when the real symbol is decompiled.
- Do not stretch UI atlas textures (`fe_1_EA_b`) as full-screen backgrounds — use decomp/layout paths.
- Do not commit `disc/`, `assets/` extracts, or ISOs (see `scripts/check_no_game_assets.sh`).

## References

- `docs/NATIVE_PORT.md` — HAL detail and phased roadmap
- `include/platform/host_syscalls.h` — symbol ownership map
- `Main Workflow.txt` — splat / objdiff routine
