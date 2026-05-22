.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIState_hideObjSafe
.ent cUIState_hideObjSafe
cUIState_hideObjSafe:
    .word  0x27bdffd0
    .word  0x24840024
    .word  0x7fb10010
    .word  0x7fb00020
    .word  0xffbf0000
    .word  0x0c0e5e10
    .word  0x00a0882d
    .word  0x0040802d
    .word  0x52000016
    .word  0x7bb00020
    .word  0x0c0c5d9c
    .word  0x0220202d
    .word  0x0040282d
    .word  0x0c0e7618
    .word  0x0200202d
    .word  0x0040302d
    .word  0x10c00008
    .word  0x0000282d
    .word  0x8cc20008
    .word  0x84440048
    .word  0x8c43004c
    .word  0x0060f809
    .word  0x00c42021
    .word  0x10000007
    .word  0x7bb00020
    .word  0x8e100004
    .word  0x0c0e5dc6
    .word  0x0200202d
    .word  0x1040ffed
    .word  0x00000000
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cUIState_hideObjSafe
