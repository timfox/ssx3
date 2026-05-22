#ifndef PLATFORM_HOST_WORLD_H
#define PLATFORM_HOST_WORLD_H

#ifdef __cplusplus
extern "C" {
#endif

void* host_world_create(void);
void host_world_destroy(void* self);
void* host_world_view_create(int num_sections);
void host_world_view_destroy(void* view);
void* host_world_cache_create(void);
void host_world_cache_destroy(void* cache);
void* host_stream_man_create(void);
void host_stream_man_destroy(void* self);

void* cWorld_cWorld(void* self);
int cWorld_resetMap(void* world, int map_id);
int cWorldView_getNumSections(void* view);
int cWorldView_isSectionLoaded(void* view, int section);
void cWorldCache_init(void* cache, void* world);
int cWorldCache_activateSectionMem(void* cache, void* world, int section);
void* cStreamMan_cStreamMan(void* self);
void cSectionMan_setSky(int section, int sky_id);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_WORLD_H */
