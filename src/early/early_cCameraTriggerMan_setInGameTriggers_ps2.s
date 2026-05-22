.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerMan_setInGameTriggers
.ent cCameraTriggerMan_setInGameTriggers
cCameraTriggerMan_setInGameTriggers:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0x0c05b3d2
    .word  0x24050002
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cCameraTriggerMan_setInGameTriggers
