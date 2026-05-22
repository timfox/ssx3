.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCamera_resetChaseControllerSwitches
.ent cCamera_resetChaseControllerSwitches
cCamera_resetChaseControllerSwitches:
    .word  0x27bdffe0
    .word  0x8f82f7b8
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x8c430084
    .word  0x8c700084
    .word  0x5200000c
    .word  0x7bb00010
    .word  0x8e020004
    .word  0x0c05884e
    .word  0x8c4400a8
    .word  0x8e020000
    .word  0x28420002
    .word  0x54400005
    .word  0x7bb00010
    .word  0x8e020008
    .word  0x0c05884e
    .word  0x8c4400a8
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cCamera_resetChaseControllerSwitches
