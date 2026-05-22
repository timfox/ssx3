.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBERewardInterface_isBetterMedal
.ent cBERewardInterface_isBetterMedal
cBERewardInterface_isBetterMedal:
    .word  0x2402ffff
    .word  0x14c20003
    .word  0x00000000
    .word  0x03e00008
    .word  0x0000102d
    .word  0x14a20003
    .word  0x00c5102a
    .word  0x03e00008
    .word  0x24020001
    .word  0x03e00008
    .word  0x00000000
    .word  0x00000000
.end cBERewardInterface_isBetterMedal
