#ifndef PLATFORM_HOST_FE_MAINMENU_H
#define PLATFORM_HOST_FE_MAINMENU_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_mainmenu_create(const char* asset_dir);
void host_fe_mainmenu_destroy(void* self);

void cFEStateMainMenu_onCreateScreen(void* self);
void cFEStateMainMenu_onDestroyScreen(void* self);
void cFEStateMainMenu_onUpdate(void* self);
void cFEStateMainMenu_onWidgetCreate(void* self, void* widget);
void cFEStateMainMenu_onWidgetEvent(void* self, void* widget, int event);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_MAINMENU_H */
