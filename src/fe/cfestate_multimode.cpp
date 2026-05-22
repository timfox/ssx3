// Retail cFEStateSelectMultiplayerMode @ 0x1A3090 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_submenu_state.h"

extern "C" {
#endif

void cFEStateSelectMultiplayerMode_onCreateScreen(void* self) {
    host::fe_submenu_on_create_screen(
        host::fe_submenu_as_state(self, host::kFEMultiplayMagic),
        "cFEStateSelectMultiplayerMode_onCreateScreen",
        "0x1A3090");
}

void cFEStateSelectMultiplayerMode_onDestroyScreen(void* self) {
    host::fe_submenu_on_destroy_screen(host::fe_submenu_as_state(self, host::kFEMultiplayMagic));
}

void cFEStateSelectMultiplayerMode_onUpdate(void* self) {
    host::fe_submenu_on_update_list(host::fe_submenu_as_state(self, host::kFEMultiplayMagic));
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
