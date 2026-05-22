.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cGameComm_construct
.ent cGameComm_construct
cGameComm_construct:
    .word  0x27bdffd0
    .word  0x0000382d
    .word  0x7fb10010
    .word  0x3c062000
    .word  0x7fb00020
    .word  0x00a0882d
    .word  0x0080802d
    .word  0x3c050048
    .word  0xffbf0000
    .word  0x240405f0
    .word  0x0c0c5f5c
    .word  0x24a504b8
    .word  0x0200282d
    .word  0x0220302d
    .word  0x0c095b1e
    .word  0x0040202d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0xaf82fdfc
    .word  0x03e00008
    .word  0x27bd0030
.end cGameComm_construct
