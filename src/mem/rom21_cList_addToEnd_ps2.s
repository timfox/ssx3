.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cList_addToEnd
.ent cList_addToEnd
cList_addToEnd:
    .word  0x8c82000c
    .word  0xaca20000
    .word  0x8c430004
    .word  0xaca30004
    .word  0xac450004
    .word  0x03e00008
    .word  0xac85000c
    .word  0x00000000
.end cList_addToEnd
