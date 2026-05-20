#ifndef PLATFORM_HOST_GRAPHICS_H
#define PLATFORM_HOST_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

void cVisualEffectsMainMenu_cVisualEffectsMainMenu(void* fx, void* a1);
void cVisualEffectsMainMenu_render(void* fx, void* menu_frame);

void cRenderStateMan_SnowFlakeColourR(void* state, float value);
void cRenderStateMan_SnowFlakeColourG(void* state, float value);
void cRenderStateMan_SnowFlakeColourB(void* state, float value);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_GRAPHICS_H */
