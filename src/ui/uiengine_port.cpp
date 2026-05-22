// Retail cUIEngine @ 0x397C40 (rom21 segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_ui_engine_state.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

bool lui_has_position_prop(const std::vector<std::uint8_t>& data, std::size_t poff) {
    const std::size_t end = std::min(data.size(), poff + 512);
    for (std::size_t qoff = poff + 16; qoff + 12 < end; qoff += 16) {
        if (data[qoff] != 'Q' || data[qoff + 1] != 0) {
            break;
        }
        std::uint16_t qid = 0;
        std::uint16_t tag = 0;
        std::uint16_t typ = 0;
        std::memcpy(&qid, &data[qoff + 2], 2);
        std::memcpy(&tag, &data[qoff + 4], 2);
        std::memcpy(&typ, &data[qoff + 6], 2);
        if (tag == 16 && typ == 2 && qid == 6) {
            return true;
        }
    }
    return false;
}

int parse_lui_widget_count(const std::vector<std::uint8_t>& data) {
    if (data.size() < 28 || data[0] != 'I' || data[1] != 'Y' || data[2] != 'F') {
        return -1;
    }

    int count = 0;
    for (std::size_t off = 0; off + 24 < data.size(); ++off) {
        if (data[off] != 'P' || data[off + 1] != 0 || data[off + 2] != 8 || data[off + 3] != 0) {
            continue;
        }
        if (lui_has_position_prop(data, off)) {
            ++count;
        }
    }
    return count;
}

std::string normalize_ui_path(const char* path) {
    std::string out = path ? path : "";
    for (char& ch : out) {
        if (ch == '\\') {
            ch = '/';
        }
    }
    return out;
}

} // namespace

extern "C" {
#endif

int cUIEngine_loadFile(void* engine, const char* path) {
    auto* ui = host::fe_ui_engine_as_state(engine);
    if (!ui || !path) {
        return 0;
    }

    const std::string disc_path = normalize_ui_path(path);
    const auto bytes = host::read_disc_file(disc_path);
    if (!bytes || bytes->empty()) {
        host::host_log("ui", ("cUIEngine_loadFile failed: " + disc_path).c_str());
        return 0;
    }

    const int widgets = parse_lui_widget_count(*bytes);
    if (widgets < 0) {
        host::host_log("ui", ("cUIEngine_loadFile: not IYF — " + disc_path).c_str());
        return 0;
    }

    ui->last_widget_count = widgets;
    std::strncpy(ui->last_path, disc_path.c_str(), sizeof(ui->last_path) - 1);

    char log_buf[160];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "cUIEngine_loadFile %s: %d P-widgets (retail 0x397C40)",
                  disc_path.c_str(),
                  widgets);
    host::host_log("ui", log_buf);
    return 1;
}

int cUIEngine_addScreenByHashName(void* engine, unsigned int screen_hash) {
    auto* ui = host::fe_ui_engine_as_state(engine);
    if (!ui) {
        return 0;
    }
    char log_buf[96];
    std::snprintf(log_buf,
                  sizeof(log_buf),
                  "cUIEngine_addScreenByHashName 0x%08X (retail 0x397CC0)",
                  screen_hash);
    host::host_log("ui", log_buf);
    return 1;
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
