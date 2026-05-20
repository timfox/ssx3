#ifndef PLATFORM_HOST_BOOT_SEQUENCE_H
#define PLATFORM_HOST_BOOT_SEQUENCE_H

#include <string>

namespace host {

class Renderer;

bool run_boot_video_sequence(const std::string& disc_root, Renderer* display);

} // namespace host

#endif /* PLATFORM_HOST_BOOT_SEQUENCE_H */
