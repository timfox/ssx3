#ifndef PLATFORM_HOST_INPUT_H
#define PLATFORM_HOST_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_input_map_create(void);
void host_input_map_destroy(void* self);

void cInputMap_init(void* self);
int cInputMap_loadMapFile(void* self, const char* path);
int cInputMap_compileMap(void* self);
void cInputMap_purgeMapFile(void* self);
int cSSXApp_loadInputMap(void* app, const char* path);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_INPUT_H */
