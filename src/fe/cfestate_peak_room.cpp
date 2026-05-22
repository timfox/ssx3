// Retail cFEStatePeakRoom @ 0x1D3890 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_submenu_state.h"
#include "platform/host_log.h"

extern "C" {
#endif

void cFEStatePeakRoom_onCreateScreen(void* self) {
    host::fe_submenu_on_create_screen(
        host::fe_submenu_as_state(self, host::kFEPeakRoomMagic),
        "cFEStatePeakRoom_onCreateScreen",
        "0x1D3890");
}

void cFEStatePeakRoom_onDestroyScreen(void* self) {
    host::fe_submenu_on_destroy_screen(host::fe_submenu_as_state(self, host::kFEPeakRoomMagic));
}

void cFEStatePeakRoom_onUpdate(void* self) {
    host::fe_submenu_on_update_list(host::fe_submenu_as_state(self, host::kFEPeakRoomMagic));
}

void cFEStatePeakRoom_onWidgetCreate(void* self, void* widget) {
    (void)self;
    (void)widget;
    host::host_log("fe", "cFEStatePeakRoom_onWidgetCreate (retail 0x1D3AD0, stub)");
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
