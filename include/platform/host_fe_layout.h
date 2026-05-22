#ifndef PLATFORM_HOST_FE_LAYOUT_H
#define PLATFORM_HOST_FE_LAYOUT_H

/* GPU texture slots for menu sprites (must match VulkanMenuGpu::load_assets). */
namespace host {

constexpr int kTexMenuLogo = 0;
constexpr int kTexMenuMountain = 1;
constexpr int kTexMenuSnow = 2;
constexpr int kTexMenuSolid = 3;
constexpr int kTexFeFont = 4;
constexpr int kTexFeIcons = 5;
constexpr int kTexMenuWidg = 6;
constexpr int kTexMenuBgPrimary = 7;   /* fe_fe_1 — warm menu backdrop */
constexpr int kTexMenuBgAccent = 8;    /* ui_PS2.png — pad hint icons (fallback: fe_1_FE_t) */
constexpr int kTexMenuOverlay = 9;     /* ui_over */
constexpr int kTexMenuHudPanel = 10;   /* ui_hudp — options panels */
constexpr int kTexMenuBrandLogo = 11;  /* ssx3_logo — corner branding */
constexpr int kTexMapPanorama = 12;    /* map_mtn / peak map from mapgfx.big */
constexpr int kTexMapPeakA = 13;
constexpr int kTexMapPeakB = 14;
constexpr int kTexMapPeakC = 15;

/* Retail fefont.sfn: ~20px caps, ~34px line step in 640×448 FE space. */
struct FeTextStyle {
    float scale = 1.0f;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct FeTitleLayout {
    static constexpr float logo_w = 548.0f;
    static constexpr float logo_y = 86.0f;
    static constexpr float press_scale = 1.62f;
    static constexpr float press_y = 296.0f;
    static constexpr float copyright_scale = 0.98f;
    static constexpr float copyright_y = 418.0f;
    static constexpr float mountain_x = -48.0f;
    static constexpr float mountain_y = 118.0f;
    static constexpr float mountain_w = 390.0f;
    static constexpr float mountain_h = 250.0f;
    static constexpr float fog_y = 252.0f;
};

struct FeMainMenuLayout {
    static constexpr float header_divider_y = 50.0f;
    static constexpr float column_divider_x = 272.0f;
    static constexpr float orange_three_x = 12.0f;
    static constexpr float orange_three_y = 8.0f;
    static constexpr float header_title_x = 20.0f;
    static constexpr float header_title_y = 14.0f;
    static constexpr float header_title_scale = 0.82f;
    static constexpr float corner_logo_w = 92.0f;
    static constexpr float corner_logo_x = 538.0f;
    static constexpr float corner_logo_y = 12.0f;
    static constexpr float desc_x = 24.0f;
    static constexpr float desc_y = 62.0f;
    static constexpr float desc_scale = 1.02f;
    static constexpr float menu_x = 292.0f;
    static constexpr float menu_y = 78.0f;
    static constexpr float menu_line_h = 34.0f;
    static constexpr float menu_scale = 1.36f;
    static constexpr float menu_scale_sel = 1.46f;
    static constexpr float sel_bar_x = 278.0f;
    static constexpr float sel_bar_w = 352.0f;
    static constexpr float sel_bar_h = 32.0f;
    static constexpr float mountain_x = 18.0f;
    static constexpr float mountain_y = 132.0f;
    static constexpr float mountain_w = 232.0f;
    static constexpr float mountain_h = 178.0f;
    static constexpr float hint_x = 468.0f;
    static constexpr float hint_y = 412.0f;
    static constexpr float hint_scale = 0.88f;
};

struct FeMountainLayout {
    static constexpr float header_scale = 2.05f;
    static constexpr float header_x = 32.0f;
    static constexpr float header_y = 22.0f;
    static constexpr float peak_scale = 1.48f;
    static constexpr float peak_scale_sel = 1.78f;
    static constexpr float peak_y = 328.0f;
    static constexpr float peak_card_w = 152.0f;
    static constexpr float peak_gap = 14.0f;
};

struct FeOptionsLayout {
    static constexpr float title_scale = 2.35f;
    static constexpr float title_x = 52.0f;
    static constexpr float title_y = 36.0f;
    static constexpr float panel_x = 36.0f;
    static constexpr float panel_y = 88.0f;
    static constexpr float panel_w = 568.0f;
    static constexpr float panel_h = 328.0f;
    static constexpr float menu_x = 88.0f;
    static constexpr float menu_y = 118.0f;
    static constexpr float menu_line_h = 38.0f;
    static constexpr float menu_scale = 1.72f;
    static constexpr float menu_scale_sel = 2.02f;
};

struct FeOptionsSubLayout {
    static constexpr float panel_x = 40.0f;
    static constexpr float panel_y = 72.0f;
    static constexpr float panel_w = 560.0f;
    static constexpr float panel_h = 344.0f;
    static constexpr float row_x = 80.0f;
    static constexpr float row_y = 112.0f;
    static constexpr float row_line_h = 36.0f;
    static constexpr float row_scale = 1.58f;
    static constexpr float row_scale_sel = 1.88f;
};

} // namespace host

#endif /* PLATFORM_HOST_FE_LAYOUT_H */
