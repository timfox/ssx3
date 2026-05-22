// Retail cWorld @ 0x3A6688 (rom21). Native HAL-backed until world.cpp is decompiled.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_world_state.h"
#include "platform/host_log.h"
#include "platform/host_object.h"

#include <cstdio>

extern "C" {
#endif

void* cWorld_cWorld(void* self) {
    auto* world = host::world_as_state(self);
    if (!world) {
        host::host_log("world", "cWorld_cWorld: invalid state");
        return self;
    }
    host::host_log("world", "cWorld_cWorld (retail 0x3A6688, native state)");
    world->map_id = -1;
    return self;
}

int cWorld_resetMap(void* world, int map_id) {
    auto* w = host::world_as_state(world);
    if (!w) {
        return -1;
    }
    w->map_id = map_id;
    ++w->reset_count;

    char log_buf[96];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "cWorld_resetMap id=%d (retail 0x3A6800, resets=%d)",
                  map_id,
                  w->reset_count);
    host::host_log("world", log_buf);
    return 0;
}

int cWorldView_getNumSections(void* view) {
    auto* wv = host::world_view_as_state(view);
    if (!wv) {
        return 0;
    }
    if (wv->guard != 0) {
        return 0;
    }
    return wv->num_sections;
}

int cWorldView_isSectionLoaded(void* view, int section) {
    auto* wv = host::world_view_as_state(view);
    if (!wv || wv->guard != 0) {
        return 0;
    }
    if (section < 0 || section >= wv->num_sections) {
        return 0;
    }
    return (wv->loaded_flags >> section) & 1;
}

void cWorldCache_init(void* cache, void* world) {
    auto* wc = host::world_cache_as_state(cache);
    auto* w = host::world_as_state(world);
    if (!wc || !w) {
        host::host_log("world", "cWorldCache_init: invalid state");
        return;
    }
    wc->world = world;
    wc->active_section = -1;
    host::host_log("world", "cWorldCache_init (retail 0x3A7FC0)");
}

int cWorldCache_activateSectionMem(void* cache, void* world, int section) {
    auto* wc = host::world_cache_as_state(cache);
    auto* w = host::world_as_state(world);
    if (!wc || !w) {
        return -1;
    }
    wc->world = world;
    wc->active_section = section;

    char log_buf[96];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "cWorldCache_activateSectionMem section=%d map=%d",
                  section,
                  w->map_id);
    host::host_log("world", log_buf);
    host_instance_man_on_section_activate(section, w->map_id);
    return 0;
}

void* cStreamMan_cStreamMan(void* self) {
    auto* sm = host::stream_man_as_state(self);
    if (!sm) {
        host::host_log("world", "cStreamMan_cStreamMan: invalid state");
        return self;
    }
    host::host_log("world", "cStreamMan_cStreamMan");
    return self;
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
