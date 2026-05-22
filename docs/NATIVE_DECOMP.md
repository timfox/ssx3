# Decompiling SSX 3 for the native (Linux) build

The native port is **not** a rewrite of SSX 3 in `src/platform/linux/`. It is the **same decompiled game C** as the PS2 target, compiled with `-DSSX3_HOST -DSKIP_ASM`, calling into a **host HAL** for OS services (disc, pad, Vulkan GS, heap, threads).

```
Retail disc  →  host HAL (disc, BIG, pad, Vulkan, OpenAL)
                      ↓
Decompiled game (`src/**`, SKIP_ASM bodies)  ← you are here
                      ↓
./out/ssx3-native
```

PS2 matching (`configure.py && ninja`) remains the correctness gate. Native (`ninja native`) grows as SKIP_ASM bodies replace placeholders and asm.

## What “done” looks like

| Milestone | Meaning |
|-----------|---------|
| **A. Runnable HAL** | `--boot-game`, `--gfx`, smoke tests (today) |
| **B. Linked decomp** | Hundreds of `src/**/*.cpp` TUs in `config/native_decomp_sources.txt` |
| **C. Boot chain** | `systemInit` → modules → `cAppMan_mainLoop` using retail logic, not stubs |
| **D. FE → race** | Title → mountain → load → in-race loop with world/render/physics decomp |

Today: **A** + **B** in progress (**53** native decomp TUs; **149** `@objdiff-matched` PS2 units in `src/mem/units/`). **C**/**D** are multi-year alongside objdiff.

## Workflow (every function)

1. Pick a symbol (`config/symbol_addrs.txt`, `asm/nonmatchings/`, or boot list in `tools/generate_ps2_units.py`).
2. Match on [decomp.me](https://decomp.me) (SSX 3) or from `*_ps2.s`.
3. Add C under `#ifdef SKIP_ASM` in the owning `src/**/*.cpp` (or a new file under `src/<area>/`).
4. Verify PS2: `venv/bin/python configure.py -o && ninja` + objdiff.
5. Refresh native link: `python3 scripts/sync_native_decomp.py`.
6. Build native: `venv/bin/python configure.py && ninja native`.
7. Run: `./out/ssx3-native disc` (smoke) or `--boot-game` / `--gfx` / `--game`.

Small leaf functions: `python3 tools/generate_ps2_units.py --boot` → embed retail opcodes in `src/mem/units/*.c` → `tools/batch_verify_units.py` (tags `// @objdiff-matched`) → `configure.py -o && ninja` uses units for `obj/current` → replace embedded asm with readable C when ready.

World stream path (ObjDiff): `cWorldCache_*`, `cStreamMan`, `cWorldSphTree`, `cWorldLightMan_initLightCache`. Game runtime (ObjDiff): `cGame_*`, `cGameViewMan_*`, `cGameModeMan_*`, `cGameComm_*`. App tick (ObjDiff): full `cSSXApp_*` boot set. Graphics (ObjDiff): `cPSPGraphicsMan_NewBindTexID`/`NewNonBindTexID`. Physics/race (ObjDiff): `cRider_*`, `cAI_*`, `cAirPredictor_*`, `cRiderAnimBase_*`, `cBENewRaceInterface_setGameMode`. World systems (ObjDiff): `cWorldPainterMan_*`, `cWorldTriggerManager_*`, `cSectionMan_setSky`. **Note:** `cGameModeHandler_run` in `game_native.cpp` is a native placeholder only — retail in-race loop goes through `cGameModeMan_getGM` / `initGameMode` (ObjDiff matched).

## Where code lives

| Layer | Path | Role |
|-------|------|------|
| Game decomp | `src/ai`, `src/object`, `src/fe`, `src/world`, … | Retail names, `SKIP_ASM` bodies |
| I/O / app | `src/bx/bigfile.cpp`, `src/main/ssxapp_port.cpp` | `cBigFile_*`, `cSSXApp_*` (HAL inside SKIP_ASM) |
| Input | `src/input/inputmap.cpp`, `src/input/inputpad.cpp` | `cInputMap_*`, `cInputPad_poll` / `getButtons` |
| FE (start) | `src/fe/cfestate_title.cpp` | `cFEStateTitle_onCreate/Update/Destroy` |
| Main | `src/main/main_native.cpp` | `ssx3_retail_main` → `run_game_boot_chain` |
| Boot / sync | `src/dcd10`, `src/retail`, `src/mem/units` | App manager, synctask, objdiff units |
| Host HAL | `src/platform/linux/host_*.cpp` | Linux only — **shrink over time** |
| Native manifest | `config/native_decomp_sources.txt` | Auto-generated link list |

**Rule:** Do not reimplement game rules in `host_*` unless bridging until the real symbol is decompiled. Prefer decomp + thin HAL syscall.

## Priority order (playability)

1. **Runtime** — `cMemMan_*`, `SYNCTASK_*`, `cAppMan_*`, `operator new/delete` (in progress; see `src/retail/`, `src/mem/`).
2. **I/O** — `cBigFile_*`, `FILESYS_*`, `BIG_*` (HAL exists; decomp `src/bx/bigfile.cpp`).
3. **Input** — `cInputMap_*`, `cInputPad_*` (HAL + maps; decomp `src/input/`).
4. **Main** — `cSSXApp_*`, `game.cpp`, `gamemodehandler.cpp` (mostly placeholders).
5. **FE** — `cFEState*` (host menu today; replace with `src/fe/` decomp).
6. **World / AI** — `cWorld_*`, `cRider_*`, physics, streaming.
7. **Render / audio** — `cPSPGraphicsMan_*`, `ps2soundman`, GS → Vulkan.

## Commands

```bash
python3 tools/native_status.py          # linked TUs, boot units, hints
python3 tools/decomp_progress.py
python3 scripts/sync_native_decomp.py
venv/bin/python configure.py && ninja native
./out/ssx3-native disc
./out/ssx3-native --boot-game disc
./out/ssx3-native --gfx disc
./out/ssx3-native --game disc
```

## References

- `docs/SOURCE_PORT.md` — architecture + phased plan
- `docs/PLAYABLE_ROADMAP.md` — objdiff + playable milestones
- `docs/NATIVE_PORT.md` — HAL modules
- `include/platform/host_syscalls.h` — symbol → host file map
