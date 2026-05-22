// Retail cInputMap — native HAL (decomp tree). PS2: rom21 @ 0x320CA0+.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_input.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <cstring>
#include <new>
#include <vector>

extern "C" {
#endif

namespace {
constexpr std::uint32_t kInputMapMagic = 0x4D504E49u; /* "INPM" */

struct HostInputMap {
    std::uint32_t magic = kInputMapMagic;
    std::vector<char> file_data;
    bool compiled = false;
};

HostInputMap* as_map(void* self) {
    return static_cast<HostInputMap*>(self);
}
} // namespace

void cInputMap_init(void* self) {
    auto* map = as_map(self);
    if (!map) {
        return;
    }
    map->magic = kInputMapMagic;
    map->file_data.clear();
    map->compiled = false;
}

int cInputMap_loadMapFile(void* self, const char* path) {
    auto* map = as_map(self);
    if (!map || !path) {
        return 0;
    }
    cInputMap_purgeMapFile(self);

    const auto bytes = host::read_asset_bytes(path);
    if (!bytes || bytes->empty()) {
        host::host_log("input", "cInputMap_loadMapFile failed");
        return 0;
    }
    map->file_data.assign(bytes->begin(), bytes->end());
    map->compiled = false;
    host::host_log("input", "cInputMap_loadMapFile OK");
    return 1;
}

int cInputMap_compileMap(void* self) {
    auto* map = as_map(self);
    if (!map || map->file_data.empty()) {
        return 0;
    }
    map->compiled = true;
    return 1;
}

void cInputMap_purgeMapFile(void* self) {
    auto* map = as_map(self);
    if (!map) {
        return;
    }
    map->file_data.clear();
    map->compiled = false;
}

void* host_input_map_create(void) {
    return new HostInputMap();
}

void host_input_map_destroy(void* self) {
    delete as_map(self);
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
