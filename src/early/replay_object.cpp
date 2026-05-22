#include "common.h"

#ifdef SSX3_HOST
extern "C" {
#endif

// Retail @ 0x0026E6A8 — restore object state from replay stream.
INCLUDE_ASM("early/early_cReplay", cReplay_restoreObject);

#ifdef SSX3_HOST
}
#endif
