#ifndef PLATFORM_HOST_LOG_H
#define PLATFORM_HOST_LOG_H

namespace host {

void host_log(const char* tag, const char* message);
void host_stub_log(const char* symbol);

} // namespace host

#endif /* PLATFORM_HOST_LOG_H */
