.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cInstanceMan_copyInstance
.ent cInstanceMan_copyInstance
cInstanceMan_copyInstance:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x00a0802d
    .word  0xffbf0000
    .word  0x7fb10010
    .word  0x00c0282d
    .word  0x8e110078
    .word  0x0200202d
    .word  0x0c0f995d
    .word  0x240600a0
    .word  0x8e020008
    .word  0xae110078
    .word  0x34422000
    .word  0x7bb10010
    .word  0xae020008
    .word  0x7bb00020
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cInstanceMan_copyInstance
