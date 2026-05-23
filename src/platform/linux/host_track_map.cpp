#include "platform/host_track_map.h"

#include "platform/host_disc.h"
#include "platform/host_log.h"

#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

namespace host {
namespace {

constexpr int kMountainPeakCount = 3;
constexpr int kSingleEventCount = 4;

int clamp_index(int index, int count) {
    if (index < 0) {
        return 0;
    }
    if (index >= count) {
        return count - 1;
    }
    return index;
}

} // namespace

int track_load_mode_for_single_event(int event_index) {
    return clamp_index(event_index, kSingleEventCount);
}

int track_load_mode_for_peak_event(int event_index) {
    return clamp_index(event_index, kSingleEventCount);
}

int track_map_id_for_mountain_peak(int peak_index) {
    return clamp_index(peak_index, kMountainPeakCount);
}

int track_map_id_for_single_event(int event_index) {
    /* Until event→venue tables are decompiled, each event uses a distinct procedural map slot. */
    return clamp_index(event_index, kSingleEventCount);
}

int track_map_id_for_peak_event(int peak_index, int event_index) {
    (void)event_index;
    return track_map_id_for_mountain_peak(peak_index);
}

const char* track_map_label(int map_id) {
    static const char* kLabels[] = {
        "DC / Peak 1",
        "Peak 2",
        "Peak 3",
        "Event map 3",
    };
    if (map_id >= 0 && map_id < static_cast<int>(sizeof(kLabels) / sizeof(kLabels[0]))) {
        return kLabels[map_id];
    }
    static char buf[32];
    std::snprintf(buf, sizeof(buf), "map %d", map_id);
    return buf;
}

void track_log_disc_availability(int map_id) {
    char path[128];
    std::snprintf(path, sizeof(path), "data/world/map%02d.big", map_id);
    if (disc().exists(path)) {
        host_log("track", path);
        return;
    }
    std::snprintf(path, sizeof(path), "data/wd%02d.big", map_id);
    if (disc().exists(path)) {
        host_log("track", path);
        return;
    }
    const fs::path worlds = disc().root() / "data/worlds";
    std::error_code ec;
    if (fs::is_directory(worlds, ec)) {
        for (const auto& entry : fs::directory_iterator(worlds, ec)) {
            if (!entry.is_regular_file(ec)) {
                continue;
            }
            if (entry.path().extension() == ".big") {
                char log_buf[160];
                std::snprintf(log_buf,
                              sizeof(log_buf),
                              "map_id=%d using worlds/%s (no map%02d.big)",
                              map_id,
                              entry.path().filename().string().c_str(),
                              map_id);
                host_log("track", log_buf);
                return;
            }
        }
    }
    char log_buf[96];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "map_id=%d: no dedicated world BIG on disc (procedural terrain)",
                  map_id);
    host_log("track", log_buf);
}

} // namespace host
