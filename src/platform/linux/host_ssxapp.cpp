#include "platform/host_input.h"
#include "platform/host_io.h"
#include "platform/host_log.h"

#include <cstring>

namespace {
constexpr std::uint32_t kSsxAppMagic = 0x53535841u; /* "SSXA" */

struct HostSSXApp {
    std::uint32_t magic = kSsxAppMagic;
    int initialized = 0;
    int locale_ready = 0;
    int input_ready = 0;
};

HostSSXApp* as_app(void* self) {
    return static_cast<HostSSXApp*>(self);
}
} // namespace

extern "C" {

void* host_ssxapp_create(void) {
    auto* app = new HostSSXApp();
    return app;
}

void host_ssxapp_destroy(void* self) {
    delete as_app(self);
}

void cSSXApp_cSSXApp(void* self) {
    auto* app = as_app(self);
    if (!app) {
        return;
    }
    app->magic = kSsxAppMagic;
    app->initialized = 0;
    app->input_ready = 0;
    app->locale_ready = 0;
}

void cSSXApp_purge(void* self) {
    auto* app = as_app(self);
    if (!app) {
        return;
    }
    app->initialized = 0;
    app->input_ready = 0;
    app->locale_ready = 0;
}

void cSSXApp__cSSXApp(void* self) { cSSXApp_purge(self); }

int cSSXApp_initLocale(void* self) {
    (void)self;
    const auto locale = host::read_disc_file("data/config/english.loc");
    if (!locale || locale->empty()) {
        return 0;
    }
    return 1;
}

int cSSXApp_init(void* self) {
    auto* app = as_app(self);
    if (!app) {
        return 0;
    }
    if (app->initialized) {
        return 1;
    }

    if (!cSSXApp_loadInputMap(self, "data/config/input.map")) {
        ::host::host_log("ssxapp", "cSSXApp_init: input.map load failed");
        return 0;
    }
    app->input_ready = 1;

    if (!cSSXApp_initLocale(self)) {
        ::host::host_log("ssxapp", "cSSXApp_init: locale init failed (stub)");
    } else {
        app->locale_ready = 1;
    }

    app->initialized = 1;
    ::host::host_log("ssxapp", "cSSXApp_init OK (host HAL)");
    return 1;
}

void cSSXApp_preUpdate(void* self) {
    (void)self;
}

void cSSXApp_flush(void* self) { (void)self; }

int cSSXApp_parseCommandLine(void* self, int argc, char** argv) {
    (void)self;
    (void)argc;
    (void)argv;
    return 1;
}

void cSSXApp_timerCallback(void* self) { (void)self; }

} // extern "C"
