#ifndef PLATFORM_HOST_FE_FONT_H
#define PLATFORM_HOST_FE_FONT_H

#include "platform/host_fe_layout.h"
#include "platform/host_menu_frame.h"

#include <string>
#include <vector>

namespace host {

enum class HostFontMode {
    StbFallback,
    Retail,
    Custom,
    CustomWithRetailIcons,
};

/* Selects which atlas to use when multiple are present (see --font). */
enum class HostFontChoice {
    Auto,
    Retail,
    Custom,
};

void host_fe_font_set_choice(HostFontChoice choice);

bool host_fe_font_ready(const std::string& asset_dir);

bool host_fe_font_custom_ready(const std::string& asset_dir);

/* Decode disc/data/fonts/fefont.sfn → fe_font_atlas.png + fe_font_metrics.bin */
bool host_fe_font_ensure(const std::string& asset_dir);

bool host_fe_font_load(const std::string& asset_dir);

bool host_fe_font_is_loaded();

HostFontMode host_fe_font_mode();

const char* host_fe_font_mode_name();

/* PNG paths for VulkanMenuGpu::load_assets (call after host_fe_font_load). */
bool host_fe_font_gpu_paths(const std::string& asset_dir,
                            std::string& primary_png,
                            std::string& icons_png,
                            bool& load_icons);

float fe_font_line_height_virtual(float scale);

void append_fe_text(std::vector<MenuSprite>& sprites,
                    const MenuViewport& viewport,
                    const char* text,
                    float vx,
                    float vy,
                    float scale,
                    float r,
                    float g,
                    float b,
                    float a);

float measure_fe_text_width_virtual(const char* text, float scale);

} // namespace host

#endif /* PLATFORM_HOST_FE_FONT_H */
