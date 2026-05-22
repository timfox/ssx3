.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMemMan_initialize
.ent cMemMan_initialize
cMemMan_initialize:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x0080802d
    .word  0x7fb10010
    .word  0x3c040054
    .word  0x00a0882d
    .word  0xffbf0000
    .word  0x0c0f95a6
    .word  0x24848b00
    .word  0x3c040050
    .word  0x0200282d
    .word  0x0220302d
    .word  0xaf900dcc
    .word  0x2484f250
    .word  0x0c0c7fd8
    .word  0xaf910dd0
    .word  0x24020001
    .word  0x0c0c6748
    .word  0xaf820dc8
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cMemMan_initialize
