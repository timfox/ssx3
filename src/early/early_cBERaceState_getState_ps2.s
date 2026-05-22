.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBERaceState_getState
.ent cBERaceState_getState
cBERaceState_getState:
    .word  0x27bdffe0
    .word  0x8f82e13c
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x1440000c
    .word  0x0080802d
    .word  0x3c05004a
    .word  0x24040008
    .word  0x24a51230
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030046
    .word  0xac500000
    .word  0x2463abe8
    .word  0xac430004
    .word  0xaf82e13c
    .word  0x8f82e13c
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cBERaceState_getState
