.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBezierMan_construct
.ent cBezierMan_construct
cBezierMan_construct:
    .word  0x27bdfff0
    .word  0x3c050049
    .word  0xffbf0000
    .word  0x24a52ea0
    .word  0x0000302d
    .word  0x0000382d
    .word  0x0c0c5f5c
    .word  0x34049c90
    .word  0x0c0e2b98
    .word  0x0040202d
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cBezierMan_construct
