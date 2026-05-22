#ifndef PLATFORM_HOST_MENU_AUDIO_H
#define PLATFORM_HOST_MENU_AUDIO_H

#include <string>

namespace host {

enum class FEScreen;

enum class MenuSfx {
    Navigate,
    Confirm,
    Back,
    Start,
};

/* Menu WAVs live in assets/host/audio/ (not under the menu texture dir). */
const char* host_menu_audio_dir();

bool host_menu_audio_ready();

bool host_menu_audio_ready(const std::string& asset_dir);

/*
 * If WAVs are missing but disc/data/audio/audio.big exists, run
 * scripts/extract_menu_audio.py (downloads vgmstream-cli on first use).
 */
bool host_menu_audio_ensure();

bool host_menu_audio_ensure(const std::string& asset_dir);

void host_menu_audio_start();

void host_menu_audio_start(const std::string& asset_dir);
void host_menu_audio_stop();

/** Title = wind only; other FE screens = menu loop + optional stem hits. */
void host_menu_audio_on_screen(FEScreen screen);

void host_menu_audio_play_sfx(MenuSfx sfx);

} // namespace host

#endif /* PLATFORM_HOST_MENU_AUDIO_H */
