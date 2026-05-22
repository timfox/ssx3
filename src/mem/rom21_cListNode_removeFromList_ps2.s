.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cListNode_removeFromList
.ent cListNode_removeFromList
cListNode_removeFromList:
    .word  0x8c820000
    .word  0x10400006
    .word  0x8c830004
    .word  0x10440004
    .word  0x00000000
    .word  0x54640002
    .word  0xac430004
    .word  0xac420004
    .word  0x50600007
    .word  0xac840000
    .word  0x50640005
    .word  0xac840000
    .word  0x54440002
    .word  0xac620000
    .word  0xac630000
    .word  0xac840000
    .word  0x03e00008
    .word  0xac840004
.end cListNode_removeFromList
