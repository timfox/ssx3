.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_quitEvent
.ent cRider_quitEvent
cRider_quitEvent:
    .word  0x27bdffe0
    .word  0x44800800
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0xc4800470
    .word  0x46000836
    .word  0x00000000
    .word  0x45010002
    .word  0x24020001
    .word  0x0000102d
    .word  0x1440000f
    .word  0x7bb00010
    .word  0x24100001
    .word  0x8c8306c0
    .word  0xac900480
    .word  0x248506c0
    .word  0x84640038
    .word  0x8c62003c
    .word  0x0040f809
    .word  0x00a42021
    .word  0x3c030053
    .word  0x00021080
    .word  0x246366d0
    .word  0x00431021
    .word  0xac500000
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cRider_quitEvent
