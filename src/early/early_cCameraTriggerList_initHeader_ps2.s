.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerList_initHeader
.ent cCameraTriggerList_initHeader
cCameraTriggerList_initHeader:
    .word  0x24020007
    .word  0xac800010
    .word  0xac82000c
    .word  0xac800004
    .word  0x03e00008
    .word  0xac800008
.end cCameraTriggerList_initHeader
