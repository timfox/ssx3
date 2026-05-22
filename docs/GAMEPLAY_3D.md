# 3D gameplay on the Linux host port

## Current milestone (`--game`)

The native binary can run a **Vulkan 3D flyover** on procedural alpine terrain while retail world code is still being linked:

```bash
./venv/bin/python configure.py && ninja native
./out/ssx3-native --game
./out/ssx3-native --game --resolution 1080p
./out/ssx3-native --list-resolutions
```

Controls:

- **Move focus**: left stick or arrow keys / WASD
- **Orbit camera**: right stick
- **Zoom**: L1/R1 (hold), L2/R2 (step)
- **Exit**: Triangle or Escape

This path uses:

- `host_world_scene.cpp` — heightfield mesh + orbit camera
- `host_vulkan_world.cpp` — depth-tested mesh pipeline (`mesh.vert` / `mesh.frag`)
- `host_gameplay.cpp` — pad-driven session loop
- `host_world.cpp` — logging stubs for `cWorld_*` / `cWorldView_*`

## Roadmap toward retail freeride

| Phase | Goal |
|-------|------|
| A (done) | Depth + indexed mesh, MVP uniforms, procedural slope |
| B | Load section/height data from disc BIGs (extract script + `cWorld_resetMap` hook) |
| C | Link `src/world/*.cpp`, `src/render/*.cpp`, `gamerender.cpp` as decomp matures |
| D | Rider physics, camera rig, HUD (`cOVStateHUD1P_*`) |
| E | FE bridge: Mountain Room → Cross starts `--game` with selected peak |

See `docs/NATIVE_PORT.md` phase 8 for the full world/race plan.
