.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewRaceInterface_setNumberHumans
.ent cBENewRaceInterface_setNumberHumans
cBENewRaceInterface_setNumberHumans:
    .word  0x3c020053
    .word  0x0000302d
    .word  0x24425bc8
    .word  0x8c43003c
    .word  0x0040382d
    .word  0x8c440038
    .word  0x00a31821
    .word  0xac450034
    .word  0x00641821
    .word  0xac430030
    .word  0xace60000
    .word  0xace60018
    .word  0x24c60001
    .word  0x28c20006
    .word  0x00000000
    .word  0x1440fffa
    .word  0x24e70004
    .word  0x03e00008
    .word  0x00000000
    .word  0x00000000
.end cBENewRaceInterface_setNumberHumans
