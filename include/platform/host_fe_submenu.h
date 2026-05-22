#ifndef PLATFORM_HOST_FE_SUBMENU_H
#define PLATFORM_HOST_FE_SUBMENU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Previews — retail cFEStateBonusMaterial @ 0x1954D8 */
void* host_fe_previews_create(const char* asset_dir);
void host_fe_previews_destroy(void* self);
void cFEStateBonusMaterial_onCreateScreen(void* self);
void cFEStateBonusMaterial_onDestroyScreen(void* self);
void cFEStateBonusMaterial_onUpdate(void* self);

/* Multi Play — retail cFEStateSelectMultiplayerMode @ 0x1A3090 */
void* host_fe_multiplay_create(const char* asset_dir);
void host_fe_multiplay_destroy(void* self);
void cFEStateSelectMultiplayerMode_onCreateScreen(void* self);
void cFEStateSelectMultiplayerMode_onDestroyScreen(void* self);
void cFEStateSelectMultiplayerMode_onUpdate(void* self);

/* Single Event — retail cFEStateEventSelect @ 0x186658 */
void* host_fe_single_event_create(const char* asset_dir);
void host_fe_single_event_destroy(void* self);
void cFEStateEventSelect_onCreateScreen(void* self);
void cFEStateEventSelect_onDestroyScreen(void* self);
void cFEStateEventSelect_onUpdate(void* self);

/* Peak pick follow-up — retail cFEStatePeakRoom @ 0x1D3890 */
void* host_fe_peak_room_create(const char* asset_dir, int peak_index);
void host_fe_peak_room_destroy(void* self);
void cFEStatePeakRoom_onCreateScreen(void* self);
void cFEStatePeakRoom_onDestroyScreen(void* self);
void cFEStatePeakRoom_onUpdate(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_SUBMENU_H */
