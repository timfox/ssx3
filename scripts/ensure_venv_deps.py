#!/usr/bin/env python3
"""Install script deps into repo-local .venv and prepend it to sys.path."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VENV_DIR = ROOT / ".venv"


def ensure_venv_deps(*packages: str) -> None:
    venv_py = VENV_DIR / "bin" / "python"
    if not venv_py.is_file():
        print(f"[deps]    creating {VENV_DIR}")
        subprocess.run([sys.executable, "-m", "venv", str(VENV_DIR)], check=True)
    if packages:
        subprocess.run([str(venv_py), "-m", "pip", "install", "-q", *packages], check=True)
    site = VENV_DIR / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "site-packages"
    site_str = str(site)
    if site_str not in sys.path:
        sys.path.insert(0, site_str)
