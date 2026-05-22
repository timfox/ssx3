.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerList_init
.ent cCameraTriggerList_init
cCameraTriggerList_init:
    .word  0x27bdfff0
    .word  0x0080102d
    .word  0xffbf0000
    .word  0x0c05b0e4
    .word  0xac400000
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cCameraTriggerList_init
