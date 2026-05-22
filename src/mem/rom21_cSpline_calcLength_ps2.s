.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cSpline_calcLength
.ent cSpline_calcLength
cSpline_calcLength:
    .word  0x8c820008
    .word  0x24040001
    .word  0x8c430068
    .word  0x8c650020
    .word  0x0085102a
    .word  0x10400009
    .word  0x8c630024
    .word  0x24a4ffff
    .word  0x2484ffff
    .word  0x00000000
    .word  0x00000000
    .word  0x00000000
    .word  0x00000000
    .word  0x1480fffa
    .word  0x8c630064
    .word  0xc461000c
    .word  0xc4600084
    .word  0x03e00008
    .word  0x46010000
    .word  0x00000000
.end cSpline_calcLength
