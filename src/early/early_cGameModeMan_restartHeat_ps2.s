.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cGameModeMan_restartHeat
.ent cGameModeMan_restartHeat
cGameModeMan_restartHeat:
    .word  0x27bdfff0
    .word  0x3c020053
    .word  0xffbf0000
    .word  0x24426668
    .word  0x8c830004
    .word  0x00031880
    .word  0x00621821
    .word  0x8c650000
    .word  0x8ca20000
    .word  0x84440010
    .word  0x8c430014
    .word  0x0060f809
    .word  0x00a42021
    .word  0x3c020053
    .word  0x24030005
    .word  0x244266d0
    .word  0x24420014
    .word  0x00000000
    .word  0xac400000
    .word  0x2463ffff
    .word  0x00000000
    .word  0x00000000
    .word  0x00000000
    .word  0x0461fffa
    .word  0x2442fffc
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cGameModeMan_restartHeat
