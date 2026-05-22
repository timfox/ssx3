#include "platform/host_object.h"

#include "platform/host_big.h"
#include "platform/host_disc.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

extern "C" {

void** D_002AF7B8;

struct HostObjectInterface {
    char pad[0x88];
    void* instance_man;
};

struct HostInstanceSlot {
    unsigned char active;
    signed char section;
    short type_tag;
    int flags;
    float extent;
    float px;
    float py;
    float pz;
    alignas(16) char node[0xB0];
};

struct HostInstanceMan {
    unsigned int magic;
    int track_id;
    int section_id;
    int loaded;
    int instance_count;
    HostInstanceSlot slots[24];
};

constexpr unsigned int kHostInstanceManMagic = 0x494E534Du; /* "INSM" */
constexpr float kPi = 3.14159265f;
constexpr std::size_t kInstanceCatalogBytes = 0x10000;
constexpr std::size_t kInstanceCatalogTableBytes = 0xBC00;
constexpr std::size_t kInstanceCatalogSlotStride = 0x2F0;
constexpr std::uintptr_t kCatalogEeRamMin = 0x00400000u;
constexpr std::uintptr_t kCatalogEeRamMax = 0x00500000u;

extern "C" void* D_004FE770;

struct HostCatalogSpawn {
    float px;
    float py;
    float pz;
    float extent;
    int section;
};

static constexpr std::uintptr_t kCatalogEeBase = 0x004FE770u;

static std::uintptr_t host_catalog_normalize_ref(std::uintptr_t ref) {
    if (ref >= kCatalogEeBase && ref < kCatalogEeBase + kInstanceCatalogBytes) {
        return ref - kCatalogEeBase;
    }
    return ref;
}

static void host_catalog_relocate_region(char* region, std::size_t region_size) {
    if (region == nullptr || region_size < 4 || D_004FE770 == nullptr) {
        return;
    }

    for (std::size_t off = 0; off + 4 <= region_size; off += 4) {
        std::uint32_t& word = *reinterpret_cast<std::uint32_t*>(region + off);
        const std::uintptr_t normalized = host_catalog_normalize_ref(word);
        if (normalized != static_cast<std::uintptr_t>(word) && normalized < kInstanceCatalogBytes) {
            word = static_cast<std::uint32_t>(normalized);
        }
    }

    char* cursor = region;
    const char* end = region + std::min(region_size, kInstanceCatalogTableBytes);
    auto* catalog_base = static_cast<char*>(D_004FE770);
    while (cursor + kInstanceCatalogSlotStride <= end) {
        std::uint32_t& entry_ref = *reinterpret_cast<std::uint32_t*>(cursor + 0x2E0);
        entry_ref = static_cast<std::uint32_t>(host_catalog_normalize_ref(entry_ref));

        if (entry_ref < kInstanceCatalogBytes) {
            char* entry = catalog_base + entry_ref;
            std::uint32_t& matrix_ref =
                *reinterpret_cast<std::uint32_t*>(entry + 0xF8);
            matrix_ref =
                static_cast<std::uint32_t>(host_catalog_normalize_ref(matrix_ref));
        }

        cursor += kInstanceCatalogSlotStride;
    }
}

static void host_catalog_relocate_all() {
    if (D_004FE770 == nullptr) {
        return;
    }
    auto* base = static_cast<char*>(D_004FE770);
    host_catalog_relocate_region(base, kInstanceCatalogTableBytes);
    host_catalog_relocate_region(base + kInstanceCatalogTableBytes,
                                 kInstanceCatalogBytes - kInstanceCatalogTableBytes);
}

static const void* host_catalog_resolve_ptr(std::uintptr_t ref) {
    if (D_004FE770 == nullptr || ref == 0) {
        return nullptr;
    }
    ref = host_catalog_normalize_ref(ref);
    auto* base = static_cast<const char*>(D_004FE770);
    if (ref < kInstanceCatalogBytes) {
        return base + ref;
    }
    if (ref >= kCatalogEeRamMin && ref < kCatalogEeRamMax) {
        return reinterpret_cast<const void*>(ref);
    }
    return nullptr;
}

static bool host_catalog_read_translation(const float* matrix, float* x, float* y, float* z) {
    if (matrix == nullptr) {
        return false;
    }
    float tx = matrix[12];
    float ty = matrix[13];
    float tz = matrix[14];
    if (std::fabs(tx) < 0.01f && std::fabs(ty) < 0.01f && std::fabs(tz) < 0.01f) {
        tx = matrix[0];
        ty = matrix[1];
        tz = matrix[2];
    }
    const float max_abs =
        std::max({std::fabs(tx), std::fabs(ty), std::fabs(tz)});
    if (max_abs < 0.5f || max_abs > 500000.0f) {
        return false;
    }
    *x = tx;
    *y = ty;
    *z = tz;
    return true;
}

static const float* host_catalog_resolve_matrix(const void* entry) {
    if (entry == nullptr) {
        return nullptr;
    }
    const auto* bytes = static_cast<const char*>(entry);
    const std::uintptr_t matrix_ref =
        static_cast<std::uintptr_t>(*(const std::uint32_t*)(bytes + 0xF8));
    const void* indirect = host_catalog_resolve_ptr(matrix_ref);
    if (indirect != nullptr) {
        return static_cast<const float*>(indirect);
    }
    return reinterpret_cast<const float*>(bytes + 0xF8);
}

static int host_catalog_collect_spawns(HostCatalogSpawn* out, int max_out, int section_id) {
    if (out == nullptr || max_out <= 0 || D_004FE770 == nullptr) {
        return 0;
    }

    int written = 0;
    auto* base = static_cast<char*>(D_004FE770);
    for (std::size_t region = 0; region < kInstanceCatalogBytes; region += kInstanceCatalogTableBytes) {
        char* cursor = base + region;
        const char* end = cursor + kInstanceCatalogTableBytes;
        while (cursor + kInstanceCatalogSlotStride <= end && written < max_out) {
            const std::uintptr_t entry_ref =
                static_cast<std::uintptr_t>(*(const std::uint32_t*)(cursor + 0x2E0));
            const void* entry = host_catalog_resolve_ptr(entry_ref);
            if (entry == nullptr) {
                entry = cursor;
            }

            float px = 0.0f;
            float py = 0.0f;
            float pz = 0.0f;
            const float* matrix = host_catalog_resolve_matrix(entry);
            if (!host_catalog_read_translation(matrix, &px, &py, &pz)) {
                cursor += kInstanceCatalogSlotStride;
                continue;
            }

            const int slot_section = *(const int*)(cursor + 0x10);
            if (section_id >= 0 && slot_section >= 0 && slot_section != section_id) {
                cursor += kInstanceCatalogSlotStride;
                continue;
            }

            HostCatalogSpawn& spawn = out[written++];
            spawn.px = px;
            spawn.py = py;
            spawn.pz = pz;
            spawn.extent = 1.0f;
            const float bound = *(const float*)(static_cast<const char*>(entry) + 0x18);
            if (bound > 0.1f && bound < 500.0f) {
                spawn.extent = bound;
            }
            spawn.section = slot_section;

            cursor += kInstanceCatalogSlotStride;
        }
        if (written > 0) {
            break;
        }
    }
    return written;
}

static HostObjectInterface g_host_object_interface;
static HostInstanceMan g_host_instance_man;
static void* g_iface_singleton;

static float sample_terrain_height(float x, float z, int map_id) {
    const float phase = static_cast<float>(map_id) * 0.41f;
    const float ridge = 18.0f * std::sin(x * 0.018f + phase) * std::cos(z * 0.014f - phase * 0.5f) +
                        12.0f * std::sin((x + z) * 0.009f + phase * 0.25f);
    const float slope = -0.035f * z + 35.0f + static_cast<float>(map_id) * 2.0f;
    return ridge + slope;
}

static int disc_big_entry_hint(int map_id, int section_id) {
    std::vector<std::string> candidates;
    char path[128];
    std::snprintf(path, sizeof(path), "data/world/map%02d.big", map_id);
    candidates.emplace_back(path);
    std::snprintf(path, sizeof(path), "data/wd%02d.big", map_id);
    candidates.emplace_back(path);
    std::snprintf(path, sizeof(path), "data/world/sec%02d.big", section_id);
    candidates.emplace_back(path);
    candidates.emplace_back("data/objects.big");
    candidates.emplace_back("data/world/world.big");

    for (const std::string& rel : candidates) {
        if (!host::disc().exists(rel)) {
            continue;
        }
        const auto archive = host::big_open(rel);
        if (!archive || archive->entries.empty()) {
            continue;
        }
        const int entries = static_cast<int>(archive->entries.size());
        const int spawn = std::clamp(entries / 32, 2, 12);
        char log_buf[160];
        std::snprintf(log_buf,
                      sizeof(log_buf),
                      "instance_man disc hint %s entries=%d spawn=%d",
                      rel.c_str(),
                      entries,
                      spawn);
        host::host_log("object", log_buf);
        return spawn;
    }

    const auto data_children = host::disc().list_children("data");
    int best = 0;
    for (const auto& child : data_children) {
        if (child.extension() != ".big") {
            continue;
        }
        const auto rel = std::filesystem::path("data") / child.filename();
        const auto archive = host::big_open(rel.string());
        if (!archive) {
            continue;
        }
        const int entries = static_cast<int>(archive->entries.size());
        if (entries > best) {
            best = std::clamp(entries / 32, 2, 12);
        }
    }
    return best;
}

static void host_try_load_instance_catalog(int track_id) {
    if (D_004FE770 == nullptr) {
        return;
    }

    char path[128];
    std::snprintf(path, sizeof(path), "data/world/map%02d.big", track_id);
    std::vector<std::string> candidates = {
        path,
        "data/objects.big",
        "data/world/objects.big",
        "data/world/world.big",
    };

    for (const std::string& rel : candidates) {
        if (!host::disc().exists(rel)) {
            continue;
        }
        const auto archive = host::big_open(rel);
        if (!archive || archive->entries.empty()) {
            continue;
        }

        const host::BigEntry* best = nullptr;
        for (const host::BigEntry& entry : archive->entries) {
            if (entry.size < 0x800 || entry.offset + entry.size > archive->data.size()) {
                continue;
            }
            if (best == nullptr || entry.size > best->size) {
                best = &entry;
            }
        }
        if (best == nullptr) {
            continue;
        }

        const std::size_t copy_bytes =
            std::min<std::size_t>(best->size, kInstanceCatalogBytes);
        std::memset(D_004FE770, 0, kInstanceCatalogBytes);
        std::memcpy(D_004FE770,
                    reinterpret_cast<const char*>(archive->data.data()) + best->offset,
                    copy_bytes);
        if (copy_bytes > kInstanceCatalogTableBytes) {
            const std::size_t tail_bytes =
                std::min(copy_bytes - kInstanceCatalogTableBytes,
                         kInstanceCatalogBytes - kInstanceCatalogTableBytes);
            std::memcpy(static_cast<char*>(D_004FE770) + kInstanceCatalogTableBytes,
                        reinterpret_cast<const char*>(archive->data.data()) + best->offset +
                            kInstanceCatalogTableBytes,
                        tail_bytes);
        }

        host_catalog_relocate_all();

        HostCatalogSpawn probe[24];
        const int probe_count = host_catalog_collect_spawns(probe, 24, -1);

        char log_buf[192];
        std::snprintf(log_buf,
                      sizeof(log_buf),
                      "instance_man catalog %s %zu bytes slots=%d -> D_004FE770",
                      rel.c_str(),
                      copy_bytes,
                      probe_count);
        host::host_log("object", log_buf);
        return;
    }
}

static int host_spawn_section_instances(int section_id, int map_id) {
    HostCatalogSpawn catalog_spawns[24];
    int catalog_count = host_catalog_collect_spawns(catalog_spawns, 24, section_id);
    if (catalog_count == 0) {
        catalog_count = host_catalog_collect_spawns(catalog_spawns, 24, -1);
    }

    const int disc_hint = disc_big_entry_hint(map_id, section_id);
    const int fallback_count = disc_hint > 0 ? disc_hint : (2 + (section_id & 1));
    const int spawn_count = catalog_count > 0 ? catalog_count : fallback_count;
    const float radius_base = 50.0f + static_cast<float>(section_id) * 18.0f;

    for (int i = 0; i < spawn_count; i++) {
        if (g_host_instance_man.instance_count >= 24) {
            break;
        }
        HostInstanceSlot& slot = g_host_instance_man.slots[g_host_instance_man.instance_count];
        std::memset(&slot, 0, sizeof(slot));
        slot.active = 1;
        slot.section = static_cast<signed char>(section_id);
        slot.type_tag = static_cast<short>(map_id + section_id + i);
        slot.extent = 1.0f + static_cast<float>(i) * 0.15f;
        slot.flags = 0x100;

        if (catalog_count > 0) {
            const HostCatalogSpawn& spawn = catalog_spawns[i];
            slot.px = spawn.px;
            slot.py = spawn.py;
            slot.pz = spawn.pz;
            slot.extent = spawn.extent;
            if (spawn.section >= 0) {
                slot.section = static_cast<signed char>(spawn.section);
            }
        } else {
            const float angle =
                (static_cast<float>(i) / static_cast<float>(spawn_count)) * 2.0f * kPi;
            slot.px = std::cos(angle) * radius_base;
            slot.pz = std::sin(angle) * radius_base;
            slot.py = sample_terrain_height(slot.px, slot.pz, map_id) + slot.extent * 2.5f;
        }

        *(int*)(slot.node + 0xC) = 0x491C80;
        *(float*)(slot.node + 0x18) = slot.extent;
        *(int*)(slot.node + 8) = slot.flags;

        g_host_instance_man.instance_count += 1;
    }
    return catalog_count;
}

void host_object_interface_init(void) {
    std::memset(&g_host_object_interface, 0, sizeof(g_host_object_interface));
    std::memset(&g_host_instance_man, 0, sizeof(g_host_instance_man));
    g_host_instance_man.magic = kHostInstanceManMagic;
    g_host_object_interface.instance_man = &g_host_instance_man;
    g_iface_singleton = &g_host_object_interface;
    D_002AF7B8 = &g_iface_singleton;
}

void host_instance_man_on_track_load(int track_id) {
    if (g_host_instance_man.magic != kHostInstanceManMagic) {
        host_object_interface_init();
    }
    g_host_instance_man.track_id = track_id;
    g_host_instance_man.section_id = -1;
    g_host_instance_man.loaded = 1;
    g_host_instance_man.instance_count = 0;
    std::memset(g_host_instance_man.slots, 0, sizeof(g_host_instance_man.slots));
    host_try_load_instance_catalog(track_id);

    char log_buf[96];
    std::snprintf(log_buf, sizeof(log_buf),
                  "instance_man track_load id=%d (retail object graph)", track_id);
    host::host_log("object", log_buf);
}

void host_instance_man_on_section_activate(int section_id, int map_id) {
    if (g_host_instance_man.magic != kHostInstanceManMagic) {
        host_object_interface_init();
    }
    g_host_instance_man.section_id = section_id;
    g_host_instance_man.track_id = map_id;
    g_host_instance_man.loaded = 1;

    const int catalog_count = host_spawn_section_instances(section_id, map_id);

    char log_buf[128];
    std::snprintf(log_buf, sizeof(log_buf),
                  "instance_man section=%d map=%d spawned=%d catalog=%d",
                  section_id,
                  map_id,
                  g_host_instance_man.instance_count,
                  catalog_count);
    host::host_log("object", log_buf);
}

int host_instance_man_active_count(void) {
    if (g_host_instance_man.magic != kHostInstanceManMagic) {
        return 0;
    }
    return g_host_instance_man.instance_count;
}

int host_instance_man_get_markers(HostWorldMarker* out, int max_out, int map_id) {
    if (out == nullptr || max_out <= 0 || g_host_instance_man.magic != kHostInstanceManMagic) {
        return 0;
    }
    int written = 0;
    for (int i = 0; i < g_host_instance_man.instance_count && written < max_out; i++) {
        const HostInstanceSlot& slot = g_host_instance_man.slots[i];
        if (!slot.active) {
            continue;
        }
        HostWorldMarker& marker = out[written++];
        marker.px = slot.px;
        marker.py = slot.py;
        marker.pz = slot.pz;
        marker.extent = slot.extent;
        marker.section = slot.section;
        marker.type_tag = slot.type_tag;
        (void)map_id;
    }
    return written;
}

} /* extern "C" */
