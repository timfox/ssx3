# Runtime assets (not in git)

Files here are **generated from your disc** for the native host port (for example
menu textures under `assets/host/menu/`). They are **not** shipped with this
repo and must not be committed.

## Regenerate menu textures

```bash
# Requires EA-Graphics-Manager + reversebox (see scripts/extract_menu_assets.py)
python3 scripts/extract_menu_assets.py --disc disc
```

Output: `assets/host/menu/ssx3_logo.png`, `fe_fe_1.png`, etc.

## Verify nothing is tracked

```bash
scripts/check_no_game_assets.sh
```
