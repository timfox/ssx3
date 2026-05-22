#ifndef PLATFORM_HOST_RESOLUTION_H
#define PLATFORM_HOST_RESOLUTION_H

namespace host {

struct ResolutionPreset {
    const char* id;
    int width;
    int height;
    /* Suggested internal render scale when upscaling; <= 0 uses caller default. */
    float suggested_render_scale;
};

const ResolutionPreset* resolution_presets();
int resolution_preset_count();

/* Returns preset or nullptr. Id match is case-insensitive (720p, 1080p, ps2, …). */
const ResolutionPreset* resolution_preset_by_id(const char* id);

void print_resolution_presets();

} // namespace host

#endif /* PLATFORM_HOST_RESOLUTION_H */
