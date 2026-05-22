// Retail cFEStateOptionsSaveLoad @ 0x18D280 (early segment).

#include "common.h"

#ifdef SKIP_ASM
#ifdef SSX3_HOST
#include "platform/host_fe_message_state.h"
#include "platform/host_log.h"

extern "C" {
#endif

void cFEStateOptionsSaveLoad_onCreateScreen(void* self) {
    host::fe_message_on_create_screen(
        host::fe_message_as_state(self),
        "cFEStateOptionsSaveLoad_onCreateScreen",
        "0x18D280");
}

void cFEStateOptionsSaveLoad_onDestroyScreen(void* self) {
    host::fe_message_on_destroy_screen(host::fe_message_as_state(self));
}

void cFEStateOptionsSaveLoad_onUpdate(void* self) {
    host::fe_message_on_update_screen(host::fe_message_as_state(self));
}

void cFEStateOptionsSaveLoad_onWidgetCreate(void* self, void* widget) {
    (void)self;
    (void)widget;
    host::host_log("fe", "cFEStateOptionsSaveLoad_onWidgetCreate (retail 0x18D2E8, stub)");
}

void cFEStateOptionsSaveLoad_onWidgetEvent(void* self, void* widget, int event) {
    (void)self;
    (void)widget;
    (void)event;
    host::host_log("fe", "cFEStateOptionsSaveLoad_onWidgetEvent (retail 0x18D648, stub)");
}

#ifdef SSX3_HOST
}
#endif
#endif /* SKIP_ASM */
