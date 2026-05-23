#ifndef PLATFORM_HOST_TRACK_MAP_H
#define PLATFORM_HOST_TRACK_MAP_H

namespace host {

/** Retail game-mode id passed to cGame_load / cGameModeMan_initGameMode. */
int track_load_mode_for_single_event(int event_index);
int track_load_mode_for_peak_event(int event_index);

/** World map id passed to cWorld_resetMap / cGame_loadTrack. */
int track_map_id_for_mountain_peak(int peak_index);
int track_map_id_for_single_event(int event_index);
int track_map_id_for_peak_event(int peak_index, int event_index);

/** Human-readable label for logs / HUD hints. */
const char* track_map_label(int map_id);

/** Log disc paths that exist for the requested map (e.g. data/world/mapNN.big). */
void track_log_disc_availability(int map_id);

} // namespace host

#endif /* PLATFORM_HOST_TRACK_MAP_H */
