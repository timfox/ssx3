.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cGameModeMan_initGameMode
.ent cGameModeMan_initGameMode
cGameModeMan_initGameMode:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x0c053720
    .word  0x00a0882d
    .word  0x0000282d
    .word  0x0c053756
    .word  0x0040202d
    .word  0x8e030004
    .word  0x3c020053
    .word  0x24426668
    .word  0x00031880
    .word  0x00621821
    .word  0x8c650000
    .word  0x8ca20000
    .word  0x84440018
    .word  0x8c43001c
    .word  0x0060f809
    .word  0x00a42021
    .word  0xae110008
    .word  0x3c030053
    .word  0x24040002
    .word  0x80625c11
    .word  0x14440018
    .word  0xae02000c
    .word  0x2e220006
    .word  0x10400013
    .word  0x3c020048
    .word  0x00111880
    .word  0x2442c0d0
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cGameModeMan_initGameMode
