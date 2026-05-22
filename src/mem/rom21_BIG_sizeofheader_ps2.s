.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global BIG_sizeofheader
.ent BIG_sizeofheader
BIG_sizeofheader:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0xffbf0000
    .word  0x0c0f8940
    .word  0x0080802d
    .word  0x0040182d
    .word  0x24020001
    .word  0x14620007
    .word  0x0000882d
    .word  0x92020002
    .word  0x92030003
    .word  0x00021200
    .word  0x00431025
    .word  0x1000000f
    .word  0x24510004
    .word  0x1860000d
    .word  0x28620004
    .word  0x1040000b
    .word  0x2604000c
    .word  0x9202000c
    .word  0x90830001
    .word  0x90850002
    .word  0x00021600
    .word  0x00031c00
    .word  0x90860003
    .word  0x00431025
    .word  0x00052a00
    .word  0x00451025
    .word  0x00468825
    .word  0x0220102d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end BIG_sizeofheader
