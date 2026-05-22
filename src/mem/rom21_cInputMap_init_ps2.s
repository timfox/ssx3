.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cInputMap_init
.ent cInputMap_init
cInputMap_init:
    .word  0x27bdffe0
    .word  0x00a0102d
    .word  0x7fb00010
    .word  0x3c050049
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x0000302d
    .word  0x0000382d
    .word  0x00022080
    .word  0x24a5dd00
    .word  0x0c0c5f8c
    .word  0xae020000
    .word  0x8e060000
    .word  0x3c05ffff
    .word  0xae020004
    .word  0x0040202d
    .word  0x00063080
    .word  0x0c0f9912
    .word  0x34a5ffff
    .word  0xae000010
    .word  0xae000014
    .word  0xae00000c
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cInputMap_init
