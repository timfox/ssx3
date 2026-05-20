#include "platform/host_fe_options.h"

#include "platform/host_fe_context.h"
#include "platform/host_graphics.h"
#include "platform/host_log.h"
#include "platform/host_menu_layout.h"
#include "platform/host_visualfx_menu.h"

#include <cstdint>
#include <cstring>

namespace host {

constexpr std::uint32_t kOptionsMagic = 0x46454F50u; /* "FEOP" */
constexpr int kOptionsCount = 5;

struct HostFEStateOptions {
    std::uint32_t magic = kOptionsMagic;
    HostVisualEffectsMainMenu* visual_fx = nullptr;
    MenuFrame frame{};
    char asset_dir[260]{};
    int selected = 0;
    int initialized = 0;
};

HostFEStateOptions* as_options(void* self) {
    auto* opt = static_cast<HostFEStateOptions*>(self);
    if (!opt || opt->magic != kOptionsMagic) {
        return nullptr;
    }
    return opt;
}

int fe_options_item_count() { return kOptionsCount; }

int fe_options_get_selected(void* self) {
    auto* opt = as_options(self);
    return opt ? opt->selected : 0;
}

void fe_options_navigate(void* self, int delta) {
    auto* opt = as_options(self);
    if (!opt) {
        return;
    }
    int next = opt->selected + delta;
    if (next < 0) {
        next = 0;
    }
    if (next >= kOptionsCount) {
        next = kOptionsCount - 1;
    }
    opt->selected = next;
}

bool fe_options_selection_is_back(void* self) {
    return fe_options_get_selected(self) == kOptionsCount - 1;
}

} // namespace host

extern "C" {

void* host_fe_options_create(const char* asset_dir) {
    auto* opt = new host::HostFEStateOptions();
    if (asset_dir) {
        std::strncpy(opt->asset_dir, asset_dir, sizeof(opt->asset_dir) - 1);
    }
    opt->visual_fx = host::host_visualfx_mainmenu_create();
    return opt;
}

void host_fe_options_destroy(void* self) {
    auto* opt = host::as_options(self);
    if (!opt) {
        return;
    }
    if (opt->visual_fx) {
        host::host_visualfx_mainmenu_set_active(nullptr);
        host::host_visualfx_mainmenu_destroy(opt->visual_fx);
        opt->visual_fx = nullptr;
    }
    delete opt;
}

void cFEStateOptions_onCreateScreen(void* self) {
    auto* opt = host::as_options(self);
    if (!opt) {
        return;
    }
    host::host_log("fe", "cFEStateOptions_onCreateScreen (retail 0x188800)");

    if (!opt->visual_fx) {
        opt->visual_fx = host::host_visualfx_mainmenu_create();
    }
    cVisualEffectsMainMenu_cVisualEffectsMainMenu(opt->visual_fx, nullptr);
    opt->selected = 0;
    opt->initialized = 1;
}

void cFEStateOptions_onDestroyScreen(void* self) {
    auto* opt = host::as_options(self);
    if (!opt) {
        return;
    }
    host::host_visualfx_mainmenu_set_active(nullptr);
    opt->initialized = 0;
    host::host_log("fe", "cFEStateOptions_onDestroyScreen");
}

void cFEStateOptions_onUpdate(void* self) {
    auto* opt = host::as_options(self);
    if (!opt || !opt->initialized) {
        return;
    }

    const host::MenuViewport viewport =
        host::compute_title_viewport(host::fe_frame_width(), host::fe_frame_height());

    opt->frame.time_sec += host::fe_frame_dt();
    host::build_options_screen(
        opt->frame, viewport, opt->frame.time_sec, opt->selected, host::kOptionsCount);
    cVisualEffectsMainMenu_render(opt->visual_fx, &opt->frame);
}

} // extern "C"
