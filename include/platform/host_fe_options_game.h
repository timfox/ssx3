#ifndef PLATFORM_HOST_FE_OPTIONS_GAME_H
#define PLATFORM_HOST_FE_OPTIONS_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_options_game_create(const char* asset_dir);
void host_fe_options_game_destroy(void* self);

void cFEStateOptionsGame_onCreateScreen(void* self);
void cFEStateOptionsGame_onDestroyScreen(void* self);
void cFEStateOptionsGame_onUpdate(void* self);
void cFEStateOptionsGame_onWidgetEvent(void* self, void* widget, int event);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_OPTIONS_GAME_H */
