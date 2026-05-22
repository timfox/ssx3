.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global SYNCTASK_init
.ent SYNCTASK_init
SYNCTASK_init:
    .word  0x27bdfff0
    .word  0x3c040052
    .word  0xffbf0000
    .word  0x0000282d
    .word  0x2484ed98
    .word  0x0c0f9912
    .word  0x24060100
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end SYNCTASK_init
