.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cLightMan_construct
.ent cLightMan_construct
cLightMan_construct:
    .word  0x27bdffe0
    .word  0x3c050049
    .word  0x7fb00010
    .word  0x24a52ee0
    .word  0xffbf0000
    .word  0x0000302d
    .word  0x0000382d
    .word  0x0c0c5f5c
    .word  0x24040014
    .word  0x0040802d
    .word  0x0c0e2b84
    .word  0x0200202d
    .word  0x3c030049
    .word  0x0200102d
    .word  0x24633650
    .word  0xdfbf0000
    .word  0xae030000
    .word  0x7bb00010
    .word  0x03e00008
    .word  0x27bd0020
.end cLightMan_construct
