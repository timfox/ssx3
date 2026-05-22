.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBERewardInterface_getTrackMedal
.ent cBERewardInterface_getTrackMedal
cBERewardInterface_getTrackMedal:
    .word  0x24020f88
    .word  0x34039b50
    .word  0x00c22018
    .word  0x00a34818
    .word  0x3c02004a
    .word  0x27bdfff0
    .word  0x24426ca8
    .word  0x0100282d
    .word  0x00e0302d
    .word  0xffbf0000
    .word  0x00822021
    .word  0x0c0548e6
    .word  0x01242021
    .word  0x50400002
    .word  0x2402ffff
    .word  0x80420001
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cBERewardInterface_getTrackMedal
