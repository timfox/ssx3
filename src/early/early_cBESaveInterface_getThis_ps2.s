.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBESaveInterface_getThis
.ent cBESaveInterface_getThis
cBESaveInterface_getThis:
    .word  0x27bdfff0
    .word  0x8f82e12c
    .word  0x1440000d
    .word  0xffbf0000
    .word  0x3c050046
    .word  0x24040010
    .word  0x24a5a7e0
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030046
    .word  0xac400008
    .word  0x2463ad08
    .word  0xac43000c
    .word  0xaf82e12c
    .word  0xaf80e138
    .word  0x8f82e12c
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cBESaveInterface_getThis
