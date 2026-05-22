.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerStack_init
.ent cCameraTriggerStack_init
cCameraTriggerStack_init:
    .word  0xac800004
    .word  0x03e00008
    .word  0xac800000
    .word  0x00000000
.end cCameraTriggerStack_init
