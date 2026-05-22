#include "platform/host_ui_engine.h"

#include "platform/host_ui_engine_state.h"

namespace host {

HostUIEngine* fe_ui_engine_as_state(void* engine) {
    auto* ui = static_cast<HostUIEngine*>(engine);
    if (!ui || ui->magic != kUIEngineMagic) {
        return nullptr;
    }
    return ui;
}

} // namespace host

extern "C" {

void* host_ui_engine_create(void) {
    return new host::HostUIEngine();
}

void host_ui_engine_destroy(void* engine) {
    delete host::fe_ui_engine_as_state(engine);
}

} // extern "C"
