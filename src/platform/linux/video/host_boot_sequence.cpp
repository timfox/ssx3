#include "platform/host_boot_sequence.h"

#include "platform/host_renderer.h"
#include "platform/host_video_player.h"

namespace host {

bool run_boot_video_sequence(const std::string& disc_root, Renderer* display) {
    return play_boot_movies(disc_root, display);
}

} // namespace host
