.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cGameViewMan_updateAll
.ent cGameViewMan_updateAll
cGameViewMan_updateAll:
    .word  0x27bdffc0
    .word  0x7fb10020
    .word  0x7fb20010
    .word  0x7fb00030
    .word  0x0080902d
    .word  0xffbf0000
    .word  0x8e420010
    .word  0x1040000f
    .word  0x0000882d
    .word  0x26500004
    .word  0x8e050000
    .word  0x00000000
    .word  0x26310001
    .word  0x26100004
    .word  0x8ca20090
    .word  0x84440018
    .word  0x8c43001c
    .word  0x0060f809
    .word  0x00a42021
    .word  0x8e420010
    .word  0x0222102b
    .word  0x5440fff6
    .word  0x8e050000
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
    .word  0x00000000
.end cGameViewMan_updateAll
