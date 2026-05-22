.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewRaceInterface_setNumberMission
.ent cBENewRaceInterface_setNumberMission
cBENewRaceInterface_setNumberMission:
    .word  0x3c030053
    .word  0x24635bc8
    .word  0x8c620034
    .word  0x8c64003c
    .word  0xac650038
    .word  0x00441021
    .word  0x00451021
    .word  0x03e00008
    .word  0xac620030
    .word  0x00000000
.end cBENewRaceInterface_setNumberMission
