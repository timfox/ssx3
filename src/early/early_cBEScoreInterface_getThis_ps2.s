.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEScoreInterface_getThis
.ent cBEScoreInterface_getThis
cBEScoreInterface_getThis:
    .word  0x27bdfff0
    .word  0x8f82e158
    .word  0x14400010
    .word  0xffbf0000
    .word  0x3c050046
    .word  0x24040230
    .word  0x24a5a858
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0c05526a
    .word  0x0040202d
    .word  0xaf82e158
    .word  0x2403ffff
    .word  0xac43001c
    .word  0xac400008
    .word  0xac400010
    .word  0xac400014
    .word  0xac430018
    .word  0x8f82e158
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cBEScoreInterface_getThis
