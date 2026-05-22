.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_setData
.ent cUIScreen_setData
cUIScreen_setData:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x00a0802d
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x0c0ed206
    .word  0x0200202d
    .word  0x1040000f
    .word  0x3c050049
    .word  0x0040202d
    .word  0x24a53e00
    .word  0x24060100
    .word  0x0c0c5f8c
    .word  0x0000382d
    .word  0x0200202d
    .word  0xae22000c
    .word  0x0040802d
    .word  0x0c0ed1fe
    .word  0x0200282d
    .word  0x8e230014
    .word  0x34630002
    .word  0x10000005
    .word  0xae230014
    .word  0x8e220014
    .word  0x2403fffd
    .word  0x00431024
    .word  0xae220014
    .word  0xae300010
    .word  0xdfbf0000
    .word  0x8e020008
    .word  0x02021021
    .word  0xae220034
    .word  0x8e030004
    .word  0x02031821
    .word  0xae230038
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0x03e00008
    .word  0x27bd0030
.end cUIScreen_setData
