// Retail cFEStateOptionsController @ 0x18C6A0 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_message_state.h"

extern "C" {
#endif

void cFEStateOptionsController_onCreateScreen(void* self) {
    host::fe_message_on_create_screen(
        host::fe_message_as_state(self),
        "cFEStateOptionsController_onCreateScreen",
        "0x18C6A0");
}

void cFEStateOptionsController_onDestroyScreen(void* self) {
    host::fe_message_on_destroy_screen(host::fe_message_as_state(self));
}

void cFEStateOptionsController_onUpdate(void* self) {
    host::fe_message_on_update_screen(host::fe_message_as_state(self));
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
