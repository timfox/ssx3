// Retail cFEStateBonusMaterial (previews) @ 0x1954D8 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_submenu_state.h"

extern "C" {
#endif

void cFEStateBonusMaterial_onCreateScreen(void* self) {
    host::fe_submenu_on_create_screen(
        host::fe_submenu_as_state(self, host::kFEPreviewsMagic),
        "cFEStateBonusMaterial_onCreateScreen",
        "0x1954D8");
}

void cFEStateBonusMaterial_onDestroyScreen(void* self) {
    host::fe_submenu_on_destroy_screen(host::fe_submenu_as_state(self, host::kFEPreviewsMagic));
}

void cFEStateBonusMaterial_onUpdate(void* self) {
    host::fe_submenu_on_update_list(host::fe_submenu_as_state(self, host::kFEPreviewsMagic));
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
