.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEPreRaceState_getState
.ent cBEPreRaceState_getState
cBEPreRaceState_getState:
    .word  0x27bdffe0
    .word  0x8f82e168
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x1440000c
    .word  0x0080802d
    .word  0x3c050046
    .word  0x24040008
    .word  0x24a5a8a8
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030046
    .word  0xac500000
    .word  0x2463ac18
    .word  0xac430004
    .word  0xaf82e168
    .word  0x8f82e168
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cBEPreRaceState_getState
