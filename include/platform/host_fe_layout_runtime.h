#ifndef PLATFORM_HOST_FE_LAYOUT_RUNTIME_H
#define PLATFORM_HOST_FE_LAYOUT_RUNTIME_H

namespace host {

struct FeMainMenuLayoutValues {
    float header_divider_y = 50.0f;
    float column_divider_x = 272.0f;
    float orange_three_x = 12.0f;
    float orange_three_y = 8.0f;
    float header_title_x = 72.0f;
    float header_title_y = 22.0f;
    float header_title_scale = 1.42f;
    float corner_logo_w = 92.0f;
    float corner_logo_x = 538.0f;
    float corner_logo_y = 12.0f;
    float desc_x = 28.0f;
    float desc_y = 118.0f;
    float desc_scale = 1.12f;
    float menu_x = 302.0f;
    float menu_y = 108.0f;
    float menu_line_h = 36.0f;
    float menu_scale = 1.38f;
    float menu_scale_sel = 1.38f;
    float sel_bar_x = 288.0f;
    float sel_bar_w = 332.0f;
    float sel_bar_h = 30.0f;
    float mountain_x = -40.0f;
    float mountain_y = 168.0f;
    float mountain_w = 520.0f;
    float mountain_h = 280.0f;
    float hint_x = 468.0f;
    float hint_y = 412.0f;
    float hint_scale = 0.88f;
};

struct FeMountainLayoutValues {
    float header_scale = 2.05f;
    float header_x = 32.0f;
    float header_y = 22.0f;
    float peak_scale = 1.48f;
    float peak_scale_sel = 1.78f;
    float peak_y = 328.0f;
    float peak_card_w = 152.0f;
    float peak_gap = 14.0f;
    float map_panorama_x = -24.0f;
    float map_panorama_y = 40.0f;
    float map_panorama_w = 688.0f;
    float map_panorama_h = 340.0f;
};

bool fe_layout_load();
const FeMainMenuLayoutValues& fe_main_menu_layout();
const FeMountainLayoutValues& fe_mountain_layout();

} // namespace host

#endif /* PLATFORM_HOST_FE_LAYOUT_RUNTIME_H */
