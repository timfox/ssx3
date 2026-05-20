#include "platform/host_log.h"

#include <iostream>
#include <mutex>

namespace host {
namespace {

std::mutex g_log_mutex;

} // namespace

void host_log(const char* tag, const char* message) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    std::cout << "[" << tag << "] " << message << '\n';
}

void host_stub_log(const char* symbol) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    std::cout << "[stub]    " << symbol << '\n';
}

} // namespace host
