.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEMissionInterface_getThis
.ent cBEMissionInterface_getThis
cBEMissionInterface_getThis:
    .word  0x27bdfff0
    .word  0x8f82e148
    .word  0x1440000c
    .word  0xffbf0000
    .word  0x3c050046
    .word  0x24040014
    .word  0x24a5a848
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x3c030046
    .word  0xac400008
    .word  0x2463adf8
    .word  0xac43000c
    .word  0xaf82e148
    .word  0x8f82e148
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cBEMissionInterface_getThis
