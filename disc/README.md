# Disc files (not in git)

This directory holds **your own** NTSC-U SSX 3 disc extract. It is **not** part of
the repository and must not be committed.

## Setup

1. Create this folder at the repo root (already gitignored).
2. Copy files from a legally obtained disc image or extract, **or** use
   `scripts/copy_boot_movies.sh` to pull only boot FMVs from an `.iso`.

Example layout:

```
disc/
  SYSTEM.CNF
  SLUS_207.72
  pad0.000
  pad1.000
  data/
    movies/
      eabig.mpc
      Intro.mpc
      Intro_DJ.mpc
    ...
```

Boot movies must be **non-zero** `.mpc` files (empty stubs from incomplete rips
will not play). See `docs/VIDEO_CODECS.md` and `scripts/copy_boot_movies.sh`.

## Verify nothing is tracked

```bash
scripts/check_no_game_assets.sh
```
