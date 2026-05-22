.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMoveNode_setupOverlapSystem
.ent cMoveNode_setupOverlapSystem
cMoveNode_setupOverlapSystem:
    .word  0x27bdffb0
    .word  0x7fb00040
    .word  0xffbf0030
    .word  0x0080802d
    .word  0x8e02001c
    .word  0x50400002
    .word  0x0000102d
    .word  0x8c420000
    .word  0x1040000a
    .word  0x03a0282d
    .word  0x0200202d
    .word  0x0c0d415c
    .word  0x27a60020
    .word  0x8e04001c
    .word  0x0c0d4af0
    .word  0x03a0282d
    .word  0x8e04001c
    .word  0x0c0d4b0e
    .word  0xc7ac0020
    .word  0x7bb00040
    .word  0xdfbf0030
    .word  0x03e00008
    .word  0x27bd0050
    .word  0x00000000
.end cMoveNode_setupOverlapSystem
