#ifndef PLATFORM_HOST_FE_H
#define PLATFORM_HOST_FE_H

#ifdef __cplusplus
extern "C" {
#endif

void cFEStateTitle_onCreateScreen(void* self);
void cFEStateTitle_onDestroyScreen(void* self);
void cFEStateTitle_onUpdate(void* self);

void cFEStateMainMenu_onCreateScreen(void* self);
void cFEStateMainMenu_onDestroyScreen(void* self);
void cFEStateMainMenu_onUpdate(void* self);
void cFEStateMainMenu_onWidgetCreate(void* self, void* widget);
void cFEStateMainMenu_onWidgetEvent(void* self, void* widget, int event);

void cFEStateMountainRoom_onCreateScreen(void* self);
void cFEStateMountainRoom_onDestroyScreen(void* self);
void cFEStateMountainRoom_onUpdate(void* self);
void cFEStateMountainRoom_onWidgetCreate(void* self, void* widget);
void cFEStateMountainRoom_onWidgetEvent(void* self, void* widget, int event);

void cFEStateOptions_onCreateScreen(void* self);
void cFEStateOptions_onDestroyScreen(void* self);
void cFEStateOptions_onUpdate(void* self);

void cFEStateOptionsGame_onCreateScreen(void* self);
void cFEStateOptionsGame_onDestroyScreen(void* self);
void cFEStateOptionsGame_onUpdate(void* self);
void cFEStateOptionsGame_onWidgetEvent(void* self, void* widget, int event);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_H */
