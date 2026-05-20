#ifndef PLATFORM_HOST_BOOT_H
#define PLATFORM_HOST_BOOT_H

namespace host {

/* Phase 2: mirror retail main() through host HAL (not MIPS asm). */
bool run_game_boot_chain(int argc, char** argv);

} // namespace host

#endif /* PLATFORM_HOST_BOOT_H */
