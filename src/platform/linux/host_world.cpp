#include "platform/host_world.h"

#include "platform/host_world_state.h"

namespace host {

HostWorld* world_as_state(void* self) {
    auto* world = static_cast<HostWorld*>(self);
    if (!world || world->magic != kHostWorldMagic) {
        return nullptr;
    }
    return world;
}

HostWorldView* world_view_as_state(void* view) {
    auto* wv = static_cast<HostWorldView*>(view);
    if (!wv || wv->magic != kHostWorldViewMagic) {
        return nullptr;
    }
    return wv;
}

HostWorldCache* world_cache_as_state(void* cache) {
    auto* wc = static_cast<HostWorldCache*>(cache);
    if (!wc || wc->magic != kHostWorldCacheMagic) {
        return nullptr;
    }
    return wc;
}

HostStreamMan* stream_man_as_state(void* self) {
    auto* sm = static_cast<HostStreamMan*>(self);
    if (!sm || sm->magic != kHostStreamManMagic) {
        return nullptr;
    }
    return sm;
}

} // namespace host

extern "C" {

void* host_world_create(void) {
    return new host::HostWorld();
}

void host_world_destroy(void* self) {
    delete host::world_as_state(self);
}

void* host_world_view_create(int num_sections) {
    auto* view = new host::HostWorldView();
    if (num_sections > 0) {
        view->num_sections = num_sections;
    }
    return view;
}

void host_world_view_destroy(void* view) {
    delete host::world_view_as_state(view);
}

void* host_world_cache_create(void) {
    return new host::HostWorldCache();
}

void host_world_cache_destroy(void* cache) {
    delete host::world_cache_as_state(cache);
}

void* host_stream_man_create(void) {
    return new host::HostStreamMan();
}

void host_stream_man_destroy(void* self) {
    delete host::stream_man_as_state(self);
}

} // extern "C"
