.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBE_setState
.ent cBE_setState
cBE_setState:
    .word  0x27bdffd0
    .word  0x8f82e104
    .word  0x7fb10010
    .word  0x7fb00020
    .word  0x0080882d
    .word  0x10510014
    .word  0xffbf0000
    .word  0x3c100053
    .word  0x00021080
    .word  0x261008a8
    .word  0x00501021
    .word  0x8c460000
    .word  0x8cc30004
    .word  0x84640018
    .word  0x8c65001c
    .word  0x00a0f809
    .word  0x00c42021
    .word  0x00111080
    .word  0xaf91e104
    .word  0x00501021
    .word  0x8c450000
    .word  0x8ca30004
    .word  0x84640020
    .word  0x8c620024
    .word  0x0040f809
    .word  0x00a42021
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cBE_setState
