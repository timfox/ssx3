#include "platform/host_resolution.h"

#include <cctype>
#include <cstring>
#include <iostream>

namespace host {
namespace {

constexpr ResolutionPreset kPresets[] = {
    {"ps2", 640, 448, 1.0f},
    {"720p", 1280, 720, 0.67f},
    {"wxga", 1280, 800, 0.67f},
    {"1280x896", 1280, 896, 0.67f},
    {"900p", 1600, 900, 0.6f},
    {"1080p", 1920, 1080, 0.67f},
    {"1200p", 1920, 1200, 0.6f},
    {"1440p", 2560, 1440, 0.5f},
    {"4k", 3840, 2160, 0.5f},
};

bool iequals(const char* a, const char* b) {
    if (!a || !b) {
        return false;
    }
    while (*a && *b) {
        if (std::tolower(static_cast<unsigned char>(*a)) !=
            std::tolower(static_cast<unsigned char>(*b))) {
            return false;
        }
        ++a;
        ++b;
    }
    return *a == *b;
}

} // namespace

const ResolutionPreset* resolution_presets() { return kPresets; }

int resolution_preset_count() { return static_cast<int>(sizeof(kPresets) / sizeof(kPresets[0])); }

const ResolutionPreset* resolution_preset_by_id(const char* id) {
    if (!id || !*id) {
        return nullptr;
    }
    for (const ResolutionPreset& preset : kPresets) {
        if (iequals(id, preset.id)) {
            return &preset;
        }
    }
    return nullptr;
}

void print_resolution_presets() {
    std::cout << "Resolution presets (--resolution <id>; override with --width / --height):\n";
    for (const ResolutionPreset& preset : kPresets) {
        std::cout << "  " << preset.id << "  " << preset.width << "x" << preset.height;
        if (preset.suggested_render_scale > 0.0f) {
            std::cout << "  (suggested --render-scale " << preset.suggested_render_scale << ")";
        }
        std::cout << '\n';
    }
}

} // namespace host
