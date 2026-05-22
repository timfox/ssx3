// Retail cFEStateOnlineMainMenu (host stub; no dedicated early onCreate in manifest).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_message_state.h"

extern "C" {
#endif

void cFEStateOnlineMainMenu_onCreateScreen(void* self) {
    host::fe_message_on_create_screen(
        host::fe_message_as_state(self),
        "cFEStateOnlineMainMenu_onCreateScreen",
        "online stub");
}

void cFEStateOnlineMainMenu_onDestroyScreen(void* self) {
    host::fe_message_on_destroy_screen(host::fe_message_as_state(self));
}

void cFEStateOnlineMainMenu_onUpdate(void* self) {
    host::fe_message_on_update_screen(host::fe_message_as_state(self));
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
