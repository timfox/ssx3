# Disc asset extraction (host port)

Retail SSX3 ships front-end data under `disc/data/`. The native port does not parse `.loc` / `.lui` at runtime yet; extract scripts bake assets and string tables into `assets/host/`.

## One-shot

```bash
python3 scripts/extract_host_assets.py --disc disc
```

Steps: locale → font → UI SSH archives → LUI/config copy → LUI probe JSON → UI BIG portraits/maps → ELF FE symbol index → title menu PNGs → audio.big inventory + menu WAVs → disc manifest.

## Scripts

| Script | Input | Output |
|--------|--------|--------|
| `extract_locale.py` | `data/locale/*.loc` | `assets/host/locale/*.json`, `fe_strings.json` |
| `extract_menu_font.py` | `data/fonts/fefont.sfn` | `assets/host/menu/fe_font_*` |
| `extract_ui_archives.py` | `fe_1`, `fl_1`, `gl_1`, `ov_1`, `su_1`, `splash`, `menu` SSH | `assets/host/ui/`, menu aliases |
| `extract_ui_layouts.py` | `*.lui`, `input2.map` | `assets/host/ui/layouts/`, `assets/host/config/` |
| `extract_menu_assets.py` | `fe_1.ssh` (title subset) | `assets/host/menu/` |
| `extract_audio_big.py` | `audio.big` | inventory + menu WAVs via `extract_menu_audio.py` |
| `parse_lui.py` | `*.lui` | `assets/host/ui/lui_probe/*_probe.json` |
| `extract_ui_big.py` | `charpic.big`, `mapgfx.big`, `courspic.big` | `assets/host/characters/`, `map/`, `course/` |
| `extract_elf_fe_strings.py` | `out/SLUS_207.72` | `assets/host/decomp/fe_symbols.json` |
| `export_fe_layout.py` | lui probe + retail constants | `assets/host/ui/fe_layout.json` |
| `extract_disc_manifest.py` | `disc/data/` | `assets/host/disc_manifest.json` |

The native port loads `fe_layout.json` at runtime (`host_fe_layout_runtime.cpp`) and uses
`assets/host/map/` textures in the mountain room (peak maps from `mapgfx.big`).

## Locale split (NTSC)

- **`cmnamer.loc`** — menu labels (`Single Event`, `Conquer The Mountain`, `Peak 1`, …).
- **`feamer.loc`** — descriptions and UI copy (`Play any unlocked Single Event.`, `Press START button`, …).
- **`ovamer.loc`** — overlay / HUD strings.

Curated keys for the Vulkan FE stub live in `scripts/lib/locale_extract.py` → `assets/host/locale/fe_strings.json`, loaded by `host_fe_locale.cpp`.

## Decomp map

Symbols in `config/symbol_addrs.txt` point at retail FE code not yet linked:

| Symbol | Role |
|--------|------|
| `cFELocale_addFile` | Load `.loc` pools |
| `cUIEngine_loadFile` | Load `.lui` layouts |
| `cFEStateMainMenu_onWidgetCreate` | Main menu widget tree |

Host stand-in: `host_fe_session.cpp` + `host_fe_strings.cpp` until `src/fe/*.cpp` placeholders are replaced with decomp.

## Dependencies

- Python venv: `scripts/ensure_venv_deps.py` (`reversebox`, `Pillow`)
- [EA-Graphics-Manager](https://github.com/bartlomiejduda/EA-Graphics-Manager) → `/tmp/EA-Graphics-Manager`
- [EA-Font-Manager](https://github.com/bartlomiejduda/EA-Font-Manager) → `/tmp/EA-Font-Manager`
- `tools/vgmstream/vgmstream-cli` (auto-download on first audio extract)
