.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global releaserequest
.ent releaserequest
releaserequest:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0080802d
    .word  0x8e020010
    .word  0x5040000a
    .word  0x92020000
    .word  0x8e040014
    .word  0x2c820002
    .word  0x54400006
    .word  0x92020000
    .word  0x3c030045
    .word  0x8c620c18
    .word  0x0040f809
    .word  0x00000000
    .word  0x92020000
    .word  0x3c040052
    .word  0xae00001c
    .word  0x0200282d
    .word  0xae020000
    .word  0x0c0f7b9c
    .word  0x24849bf8
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end releaserequest
