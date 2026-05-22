#ifndef PLATFORM_HOST_FE_ATLAS_H
#define PLATFORM_HOST_FE_ATLAS_H

namespace host {

/* Normalized UV rects in ssx3_logo.png / title_logo.png (512-wide sources). */
struct FeAtlasRect {
    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = 1.0f;
    float v1 = 1.0f;
};

struct FeBrandAtlas {
    /* Orange “3” on the right side of ssx3_logo.png */
    static constexpr FeAtlasRect kOrangeThree{0.56f, 0.12f, 0.96f, 0.78f};
    /* Icy “SSX” wordmark (top-left of title_logo / ssxt band) */
    static constexpr FeAtlasRect kWordmark{0.02f, 0.02f, 0.52f, 0.42f};
};

struct FeOverlayAtlas {
    /* Tree-line strip along bottom of ui/over entry */
    static constexpr FeAtlasRect kTreeLine{0.0f, 0.82f, 1.0f, 1.0f};
};

/* menu_panel.png (fe_1 Widg) — orange list row chrome */
struct FeWidgAtlas {
    static constexpr FeAtlasRect kListHighlight{0.02f, 0.38f, 0.98f, 0.58f};
};

/* ui_PS2.png / fe_1_PS2 — 128×128, four 64×64 face buttons (retail hint row). */
struct FePs2PadAtlas {
    static constexpr FeAtlasRect kTriangle{0.0f, 0.0f, 0.5f, 0.5f};
    static constexpr FeAtlasRect kCircle{0.5f, 0.0f, 1.0f, 0.5f};
    static constexpr FeAtlasRect kCross{0.0f, 0.5f, 0.5f, 1.0f};
    static constexpr FeAtlasRect kSquare{0.5f, 0.5f, 1.0f, 1.0f};
};

} // namespace host

#endif /* PLATFORM_HOST_FE_ATLAS_H */
