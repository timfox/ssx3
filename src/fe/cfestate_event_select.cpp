// Retail cFEStateEventSelect @ 0x186658 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_submenu_state.h"

extern "C" {
#endif

void cFEStateEventSelect_onCreateScreen(void* self) {
    host::fe_submenu_on_create_screen(
        host::fe_submenu_as_state(self, host::kFESingleEventMagic),
        "cFEStateEventSelect_onCreateScreen",
        "0x186658");
}

void cFEStateEventSelect_onDestroyScreen(void* self) {
    host::fe_submenu_on_destroy_screen(host::fe_submenu_as_state(self, host::kFESingleEventMagic));
}

void cFEStateEventSelect_onUpdate(void* self) {
    host::fe_submenu_on_update_list(host::fe_submenu_as_state(self, host::kFESingleEventMagic));
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
