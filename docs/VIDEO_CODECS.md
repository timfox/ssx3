# Host video codecs (license notes)

SSX3 boot and FMV assets use Electronic Arts container formats documented on
[MultimediaWiki](https://wiki.multimedia.cx/index.php/Electronic_Arts_Formats).

| Extension | Container | Video codec | SSX3 usage |
|-----------|-----------|-------------|------------|
| `.mpc` | EA chunks (`MPCh` video, `SCDl` audio, `SCHl` header) | MPEG-2 video + EA ADPCM R3 audio | `data/movies/eabig.mpc`, `intro.mpc`, … |
| `.vp6` | EA chunks (`MVhd`, `MV0K`, `MV0F`) | On2 VP6 bitstream | Later EA titles; supported for tooling |
| `.bik` | RAD Bink container | Bink video | Other EA assets; optional |

## What this repo implements

1. **In-tree demuxers** — EA chunk reader, MPC and VP6 packet extractors written from
   public format descriptions (no RAD or On2 SDK code).

2. **Optional FFmpeg backend** (`-DSSX3_HAVE_FFMPEG`) — demuxes interleaved `MPCh` /
   `SCDl` chunks and decodes with LGPL `libavcodec` (`mpeg2video`, `adpcm_ea_r3`,
   `vp6`, `binkvideo`). Audio is played via SDL.
   This is the same approach used by many open-source game tools; we do **not**
   ship or link proprietary Bink/VP6 SDKs.

3. **No SDK redistribution** — Install FFmpeg development packages on your system;
   the build detects them via `pkg-config`.

## Dependencies (Debian/Ubuntu)

```bash
apt install -y libavcodec-dev libavutil-dev libswscale-dev libswresample-dev libavformat-dev
```

## Boot movie order (NTSC-U)

1. `data/movies/eabig.mpc` — EA BIG logo
2. Intro — prefers `Intro_DJ.mpc` (DJ / announcer mix) when non-zero, else `Intro.mpc`

Copy from a full rip or ISO:

```bash
scripts/copy_boot_movies.sh "/path/to/SSX 3 (USA).iso"
# or: scripts/copy_boot_movies.sh /path/to/extracted/disc
```

Movies must be present on the extracted disc (non-zero `.mpc` files). Placeholder
zero-byte stubs from incomplete rips will be reported as errors.

## Patents

VP6 and some MPEG-2 uses may be patent-encumbered in your jurisdiction. Decoding
is performed by your locally installed FFmpeg; this project does not grant patent
licenses.
