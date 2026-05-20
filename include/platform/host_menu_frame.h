#ifndef PLATFORM_HOST_MENU_FRAME_H
#define PLATFORM_HOST_MENU_FRAME_H

#include <vector>

namespace host {

struct MenuViewport {
    int window_width = 0;
    int window_height = 0;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

struct MenuSprite {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
    float r2 = 1.0f;
    float g2 = 1.0f;
    float b2 = 1.0f;
    float a2 = 1.0f;
    bool gradient_y = false;
    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = 1.0f;
    float v1 = 1.0f;
    int texture_index = 0;
};

struct MenuFrame {
    float time_sec = 0.0f;
    int width = 0;
    int height = 0;
    MenuViewport viewport{};
    std::vector<MenuSprite> sprites;
};

} // namespace host

#endif /* PLATFORM_HOST_MENU_FRAME_H */
