#include "platform/host_bigfile.h"

#include "platform/host_big.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <cstring>

namespace {
constexpr std::uint32_t kHostBigFileMagic = 0x46494248u; /* "HBIF" */
} // namespace

extern "C" {

void cBigFile_cBigFile(HostBigFile* self) {
    if (!self) {
        return;
    }
    std::memset(self, 0, sizeof(*self));
    self->magic = kHostBigFileMagic;
}

void cBigFile__cBigFile(HostBigFile* self) {
    cBigFile_close(self);
}

int cBigFile_open(HostBigFile* self, const char* path) {
    if (!self || !path) {
        return 0;
    }
    cBigFile_close(self);
    self->magic = kHostBigFileMagic;

    std::string normalized = path;
    for (char& ch : normalized) {
        if (ch == '\\') {
            ch = '/';
        }
    }

    const bool is_big_archive =
        normalized.size() >= 4 &&
        (normalized.compare(normalized.size() - 4, 4, ".big") == 0 ||
         normalized.compare(normalized.size() - 4, 4, ".BIG") == 0);

    if (!is_big_archive && host::read_disc_file(normalized)) {
        std::strncpy(self->path, normalized.c_str(), sizeof(self->path) - 1);
        self->path[sizeof(self->path) - 1] = '\0';
        self->archive = nullptr;
        host::host_log("bigfile", "cBigFile_open loose file");
        return 1;
    }

    host::BigArchive* archive = host::big_open_cached(normalized);
    if (!archive) {
        host::host_log("bigfile", "cBigFile_open failed");
        return 0;
    }

    std::strncpy(self->path, normalized.c_str(), sizeof(self->path) - 1);
    self->path[sizeof(self->path) - 1] = '\0';
    self->archive = archive;
    host::host_log("bigfile", "cBigFile_open BIG archive");
    return 1;
}

void cBigFile_close(HostBigFile* self) {
    if (!self) {
        return;
    }
    self->magic = 0;
    self->archive = nullptr;
    self->path[0] = '\0';
}

unsigned long cBigFile_read(HostBigFile* self, const char* entry_name, void* dst, unsigned long dst_size) {
    if (!self || !entry_name || !dst || dst_size == 0 || self->magic != kHostBigFileMagic) {
        return 0;
    }
    if (!self->archive) {
        return 0;
    }
    auto* archive = static_cast<host::BigArchive*>(self->archive);
    const host::BigEntry* entry = host::big_locate(*archive, entry_name);
    if (!entry || entry->size == 0) {
        return 0;
    }
    const unsigned long to_copy = entry->size < dst_size ? entry->size : dst_size;
    if (entry->offset + to_copy > archive->data.size()) {
        return 0;
    }
    std::memcpy(dst, archive->data.data() + entry->offset, to_copy);
    return to_copy;
}

} // extern "C"
