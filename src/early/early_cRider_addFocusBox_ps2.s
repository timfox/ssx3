.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_addFocusBox
.ent cRider_addFocusBox
cRider_addFocusBox:
    .word  0x27bdffe0
    .word  0x3c050045
    .word  0x7fb00010
    .word  0x24a57960
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x24040010
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x8f84f7b8
    .word  0x0040282d
    .word  0xae02078c
    .word  0x8c820084
    .word  0x8c43000c
    .word  0x0c040cec
    .word  0x8c6400a4
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cRider_addFocusBox
