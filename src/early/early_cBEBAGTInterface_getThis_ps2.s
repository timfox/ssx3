.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEBAGTInterface_getThis
.ent cBEBAGTInterface_getThis
cBEBAGTInterface_getThis:
    .word  0x27bdfff0
    .word  0x8f82e120
    .word  0x14400011
    .word  0xffbf0000
    .word  0x3c050046
    .word  0x24040010
    .word  0x24a5a6d8
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030046
    .word  0xaf82e120
    .word  0x2463acd8
    .word  0x84640010
    .word  0x8c650014
    .word  0x00442021
    .word  0x00a0f809
    .word  0xac43000c
    .word  0x8f82e120
    .word  0xac400008
    .word  0x8f82e120
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cBEBAGTInterface_getThis
