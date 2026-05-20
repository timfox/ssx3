#ifndef PLATFORM_HOST_MENU_AUDIO_H
#define PLATFORM_HOST_MENU_AUDIO_H

#include <string>

namespace host {

enum class MenuSfx {
    Navigate,
    Confirm,
    Back,
    Start,
};

/* True when title_ambient.wav exists under asset_dir/audio/. */
bool host_menu_audio_ready(const std::string& asset_dir);

/*
 * If WAVs are missing but disc/data/audio/audio.big exists, run
 * scripts/extract_menu_audio.py (downloads vgmstream-cli on first use).
 */
bool host_menu_audio_ensure(const std::string& asset_dir);

void host_menu_audio_start(const std::string& asset_dir);
void host_menu_audio_stop();
void host_menu_audio_play_sfx(MenuSfx sfx);

} // namespace host

#endif /* PLATFORM_HOST_MENU_AUDIO_H */
