.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cStreamPause_construct
.ent cStreamPause_construct
cStreamPause_construct:
    .word  0x27bdfff0
    .word  0x3c050048
    .word  0xffbf0000
    .word  0x24040004
    .word  0x24a5c520
    .word  0x3c062000
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030048
    .word  0x0040202d
    .word  0x2463d0e8
    .word  0xdfbf0000
    .word  0xac830000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cStreamPause_construct
