.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldPainterMan_getQuery
.ent cWorldPainterMan_getQuery
cWorldPainterMan_getQuery:
    .word  0x27bdffe0
    .word  0x0000302d
    .word  0x7fb00010
    .word  0x0000382d
    .word  0x00a0802d
    .word  0xffbf0000
    .word  0x3c050048
    .word  0x24040018
    .word  0x0c0c5f5c
    .word  0x24a53de8
    .word  0x8e050000
    .word  0x0c0b0102
    .word  0x0040202d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cWorldPainterMan_getQuery
