.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewRaceInterface_setNumberAI
.ent cBENewRaceInterface_setNumberAI
cBENewRaceInterface_setNumberAI:
    .word  0x3c040053
    .word  0x2407001c
    .word  0x24845bc8
    .word  0x3c080044
    .word  0x80860049
    .word  0x2508d8f8
    .word  0x80830048
    .word  0x00c73018
    .word  0x8c820034
    .word  0x8c870038
    .word  0x00031880
    .word  0x00451021
    .word  0xac85003c
    .word  0x00471021
    .word  0x00661821
    .word  0xac820030
    .word  0x00681821
    .word  0x8c620000
    .word  0x0045102b
    .word  0x03e00008
    .word  0x38420001
    .word  0x00000000
.end cBENewRaceInterface_setNumberAI
