#ifndef PLATFORM_HOST_FE_MESSAGE_H
#define PLATFORM_HOST_FE_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_fe_message_create(const char* asset_dir, const char* title, const char* body);
void host_fe_message_destroy(void* self);

void* host_fe_online_create(const char* asset_dir);
void host_fe_online_destroy(void* self);
void cFEStateOnlineMainMenu_onCreateScreen(void* self);
void cFEStateOnlineMainMenu_onDestroyScreen(void* self);
void cFEStateOnlineMainMenu_onUpdate(void* self);

void* host_fe_options_controller_create(const char* asset_dir);
void host_fe_options_controller_destroy(void* self);
void cFEStateOptionsController_onCreateScreen(void* self);
void cFEStateOptionsController_onDestroyScreen(void* self);
void cFEStateOptionsController_onUpdate(void* self);

void* host_fe_options_saveload_create(const char* asset_dir);
void host_fe_options_saveload_destroy(void* self);
void cFEStateOptionsSaveLoad_onCreateScreen(void* self);
void cFEStateOptionsSaveLoad_onDestroyScreen(void* self);
void cFEStateOptionsSaveLoad_onUpdate(void* self);

void* host_fe_preview_detail_create(const char* asset_dir, int preview_index);
void host_fe_preview_detail_destroy(void* self);

void* host_fe_peak_event_stub_create(const char* asset_dir, int peak_index, int mode_index);
void host_fe_peak_event_stub_destroy(void* self);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_FE_MESSAGE_H */
