#ifndef PLATFORM_HOST_BOOT_H
#define PLATFORM_HOST_BOOT_H

namespace host {

/* Phase 2: mirror retail main() through host HAL (not MIPS asm). */
bool run_retail_boot_init(int argc, char** argv);
void run_retail_boot_shutdown(void);
bool run_game_boot_chain(int argc, char** argv);

/* Active retail boot session (set by run_retail_boot_init). */
void* host_boot_ssx_app(void);
void* host_boot_active_game(void);
int host_boot_race_map_id(void);
int host_boot_start_race_load(int track_id);

} // namespace host

#endif /* PLATFORM_HOST_BOOT_H */
