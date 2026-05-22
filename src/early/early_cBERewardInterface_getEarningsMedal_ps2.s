.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBERewardInterface_getEarningsMedal
.ent cBERewardInterface_getEarningsMedal
cBERewardInterface_getEarningsMedal:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x00e0802d
    .word  0x7fb20010
    .word  0x00a0882d
    .word  0xffbf0000
    .word  0x0c053720
    .word  0x00c0902d
    .word  0x0040202d
    .word  0x0c053756
    .word  0x2405000b
    .word  0x0220282d
    .word  0x0240302d
    .word  0x0c054270
    .word  0x0040202d
    .word  0x3c030046
    .word  0x00108080
    .word  0x2463b048
    .word  0x24050001
    .word  0x02038021
    .word  0x2404ffff
    .word  0x8e030000
    .word  0x7bb00030
    .word  0x0043102b
    .word  0x7bb10020
    .word  0x00a2200a
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x0080102d
    .word  0x03e00008
    .word  0x27bd0040
.end cBERewardInterface_getEarningsMedal
