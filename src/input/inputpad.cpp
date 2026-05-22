// Retail cInputPad poll/buttons — native HAL. Full pad stack in host_pad.cpp.

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_pad.h"

extern "C" {
#endif

int cInputPad_poll(int port) {
    host::pad_poll();
    return 1;
}

unsigned short cInputPad_getButtons(int port) {
    return host::pad_state(port).buttons;
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
