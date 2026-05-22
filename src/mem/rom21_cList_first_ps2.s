.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cList_first
.ent cList_first
cList_first:
    .word  0x8c820004
    .word  0x8c430004
    .word  0x00621826
    .word  0x03e00008
    .word  0x0003100a
    .word  0x00000000
.end cList_first
