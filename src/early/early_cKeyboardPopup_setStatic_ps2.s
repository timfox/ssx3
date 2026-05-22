.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cKeyboardPopup_setStatic
.ent cKeyboardPopup_setStatic
cKeyboardPopup_setStatic:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x8e020040
    .word  0x14400016
    .word  0x00a0882d
    .word  0x8e040428
    .word  0x10800004
    .word  0x00000000
    .word  0x0c0c5fa6
    .word  0x00000000
    .word  0xae000428
    .word  0x0c0f6f28
    .word  0x0220202d
    .word  0x24420001
    .word  0x3c050046
    .word  0x00022040
    .word  0x24a56590
    .word  0x24060100
    .word  0x0c0c5f8c
    .word  0x0000382d
    .word  0x0220282d
    .word  0xae020428
    .word  0x0c0f6f88
    .word  0x0040202d
    .word  0x1000000c
    .word  0x7bb00020
    .word  0x3c040046
    .word  0x0c0c5d9c
    .word  0x24846580
    .word  0x8e040040
    .word  0x0c0e7618
    .word  0x0040282d
    .word  0x10400003
    .word  0x0040202d
    .word  0x0c0e83a4
    .word  0x0220282d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cKeyboardPopup_setStatic
