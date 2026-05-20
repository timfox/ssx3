#ifndef PLATFORM_HOST_FE_MOUNTAIN_ROOM_H
#define PLATFORM_HOST_FE_MOUNTAIN_ROOM_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_mountain_room_create(const char* asset_dir);
void host_fe_mountain_room_destroy(void* self);

void cFEStateMountainRoom_onCreateScreen(void* self);
void cFEStateMountainRoom_onDestroyScreen(void* self);
void cFEStateMountainRoom_onUpdate(void* self);
void cFEStateMountainRoom_onWidgetCreate(void* self, void* widget);
void cFEStateMountainRoom_onWidgetEvent(void* self, void* widget, int event);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_MOUNTAIN_ROOM_H */
