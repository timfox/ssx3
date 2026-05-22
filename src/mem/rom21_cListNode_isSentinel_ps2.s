.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cListNode_isSentinel
.ent cListNode_isSentinel
cListNode_isSentinel:
    .word  0x8c820004
    .word  0x10440004
    .word  0x0000182d
    .word  0x8c820000
    .word  0x14440002
    .word  0x00000000
    .word  0x24030001
    .word  0x03e00008
    .word  0x0060102d
    .word  0x00000000
.end cListNode_isSentinel
