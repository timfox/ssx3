# Runtime assets (not in git)

Files here are **generated from your disc** for the native host port (for example
menu textures under `assets/host/menu/`). They are **not** shipped with this
repo and must not be committed.

## Regenerate menu textures

```bash
# Requires EA-Graphics-Manager + reversebox (see scripts/extract_menu_assets.py)
python3 scripts/extract_menu_assets.py --disc disc
```

Output for the title screen: `title_logo.png`, `title_mountain.png` (plus optional
`ui_*.png` previews). The in-game icy logo comes from `ui/fe_1.ssh` → `ssxt`, not
`splash.ssh`.

## Verify nothing is tracked

```bash
scripts/check_no_game_assets.sh
```
