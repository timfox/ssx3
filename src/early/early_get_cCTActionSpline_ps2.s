.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_cCTActionSpline
.ent get_cCTActionSpline
get_cCTActionSpline:
    .word  0x27bdffc0
    .word  0x24020002
    .word  0x7fb10020
    .word  0x7fb20010
    .word  0x00a0882d
    .word  0x7fb00030
    .word  0x0080902d
    .word  0xffbf0000
    .word  0x2625000c
    .word  0x0c0b4944
    .word  0xae220000
    .word  0x0240202d
    .word  0x26250010
    .word  0x0c0b4944
    .word  0x0040802d
    .word  0x0240202d
    .word  0x26250014
    .word  0x0c0b4944
    .word  0x02028021
    .word  0x0240202d
    .word  0x26250018
    .word  0x0c0b4944
    .word  0x02028021
    .word  0x02028021
    .word  0x0240202d
    .word  0x0c0b4944
    .word  0x2625001c
    .word  0x02021021
    .word  0x7bb10020
    .word  0x7bb00030
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
.end get_cCTActionSpline
