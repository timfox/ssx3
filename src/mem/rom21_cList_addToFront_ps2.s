.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cList_addToFront
.ent cList_addToFront
cList_addToFront:
    .word  0x8c820004
    .word  0xaca20004
    .word  0x8c430000
    .word  0xaca30000
    .word  0xac450000
    .word  0x03e00008
    .word  0xac850004
    .word  0x00000000
.end cList_addToFront
