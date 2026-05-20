#!/usr/bin/env bash
# Copy SSX3 boot FMV from a full disc tree or .iso into disc/data/movies/.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="${1:-}"
DST="$ROOT/disc/data/movies"
MOVIES=(eabig.mpc Intro.mpc Intro_DJ.mpc)

usage() {
    echo "usage: $0 /path/to/ssx3/disc/or/extract/or/game.iso"
    echo "  copies non-zero eabig.mpc, Intro.mpc, Intro_DJ.mpc into disc/data/movies/"
    exit 1
}

[[ -n "$SRC" ]] || usage

mkdir -p "$DST"
chmod u+w "$DST"/*.mpc 2>/dev/null || true

extract_from_iso() {
    local iso="$1"
    if ! command -v 7z >/dev/null; then
        echo "error: 7z required to read ISO (apt install p7zip-full)"
        exit 1
    fi
    local tmp
    tmp="$(mktemp -d)"
    trap 'rm -rf "$tmp"' EXIT
    for name in "${MOVIES[@]}"; do
        local upper="${name^^}"
        if 7z e -y -o"$tmp" "$iso" "DATA/MOVIES/$upper" >/dev/null 2>&1; then
            local extracted="$tmp/$upper"
            if [[ -f "$extracted" ]]; then
                local size
                size=$(stat -c%s "$extracted")
                if [[ "$size" -gt 0 ]]; then
                    cp -v "$extracted" "$DST/$name"
                else
                    echo "skip $name (0 bytes on ISO)"
                fi
            fi
        else
            echo "skip $name (not on ISO)"
        fi
    done
}

copy_from_dir() {
    local dir="$1"
    if [[ ! -d "$dir/data/movies" ]]; then
        echo "error: $dir/data/movies not found"
        exit 1
    fi
    for name in "${MOVIES[@]}"; do
        if [[ -f "$dir/data/movies/$name" ]]; then
            local size
            size=$(stat -c%s "$dir/data/movies/$name")
            if [[ "$size" -eq 0 ]]; then
                echo "skip $name (0 bytes in source)"
                continue
            fi
            cp -v "$dir/data/movies/$name" "$DST/"
        fi
    done
}

shopt -s nocasematch
if [[ -f "$SRC" && "$SRC" == *.iso ]]; then
    shopt -u nocasematch
    echo "extracting from ISO: $SRC"
    extract_from_iso "$SRC"
elif [[ -d "$SRC" ]]; then
    shopt -u nocasematch
    copy_from_dir "$SRC"
else
    shopt -u nocasematch
    echo "error: expected directory or .iso file: $SRC"
    exit 1
fi

echo ""
echo "Verify:"
ls -la "$DST"/eabig.mpc "$DST"/Intro.mpc "$DST"/Intro_DJ.mpc 2>/dev/null || true
echo "Boot playback prefers Intro_DJ.mpc when present (DJ intro mix)."
