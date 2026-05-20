#!/usr/bin/env bash
# Fail if git is tracking game binaries, disc data, or extracted assets.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    echo "check_no_game_assets: not a git repo, skipping"
    exit 0
fi

# Extensions and paths that must never appear in the index
PATTERNS=(
    '\.iso$'
    '\.mpc$'
    '\.big$'
    '\.vp6$'
    '\.bik$'
    '^disc/'
    '^assets/'
    '/pad0\.000$'
    '/pad1\.000$'
    '^SLUS_207\.72$'
    '^SCUS_.*\.72$'
    '^SLES_.*\.72$'
)

tracked=$(git ls-files)
violations=()

while IFS= read -r path; do
    [[ -n "$path" ]] || continue
    for pat in "${PATTERNS[@]}"; do
        if [[ "$path" =~ $pat ]]; then
            violations+=("$path")
            break
        fi
    done
done <<<"$tracked"

if ((${#violations[@]} > 0)); then
    echo "error: game content must not be tracked by git:"
    printf '  %s\n' "${violations[@]}"
    echo ""
    echo "Remove from the index (keeps local files):"
    echo "  git rm -r --cached ${violations[*]}"
    exit 1
fi

echo "ok: no game assets tracked ($(wc -l <<<"$tracked") files in index)"
