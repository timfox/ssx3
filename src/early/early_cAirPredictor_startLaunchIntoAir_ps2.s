.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAirPredictor_startLaunchIntoAir
.ent cAirPredictor_startLaunchIntoAir
cAirPredictor_startLaunchIntoAir:
    .word  0x27bdffb0
    .word  0x7fb00030
    .word  0x0080802d
    .word  0x7fb10020
    .word  0x7fb20010
    .word  0x00a0882d
    .word  0xe7b40040
    .word  0x00c0902d
    .word  0xffbf0000
    .word  0x0c044c66
    .word  0x46006506
    .word  0xe61400a8
    .word  0x0200202d
    .word  0x0220282d
    .word  0x0c044d86
    .word  0x0240302d
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0xc7b40040
    .word  0x03e00008
    .word  0x27bd0050
    .word  0x00000000
.end cAirPredictor_startLaunchIntoAir
