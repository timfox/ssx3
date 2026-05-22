#include "platform/host_log.h"

#include <chrono>
#include <thread>

namespace host {
namespace {

int g_tick_count = 0;
bool g_should_quit = false;

} // namespace
} // namespace host

extern "C" {

int host_app_tick(void) {
    if (host::g_should_quit) {
        return 0;
    }
    ++host::g_tick_count;
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    return 1;
}

void host_app_request_quit(void) { host::g_should_quit = true; }

void host_app_reset_quit(void) {
    host::g_should_quit = false;
    host::g_tick_count = 0;
}

int host_app_tick_count(void) { return host::g_tick_count; }

void SYNCTASK_yieldticks(void) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void THREAD_yieldticks(void) { SYNCTASK_yieldticks(); }

} // extern "C"
