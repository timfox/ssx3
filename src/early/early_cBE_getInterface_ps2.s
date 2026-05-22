.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBE_getInterface
.ent cBE_getInterface
cBE_getInterface:
    .word  0x8f82e104
    .word  0x27bdfff0
    .word  0x3c030053
    .word  0xffbf0000
    .word  0x246308a8
    .word  0x00021080
    .word  0x00431021
    .word  0x8c460000
    .word  0x8cc30004
    .word  0x84640008
    .word  0x8c62000c
    .word  0x0040f809
    .word  0x00c42021
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cBE_getInterface
