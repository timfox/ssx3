.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEScoreInterface_getCurrentHighlightLevel
.ent cBEScoreInterface_getCurrentHighlightLevel
cBEScoreInterface_getCurrentHighlightLevel:
    .word  0x27bdffe0
    .word  0x0000202d
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c052820
    .word  0x00a0802d
    .word  0x24040f88
    .word  0x3c03004a
    .word  0x00441018
    .word  0x24636ca8
    .word  0xdfbf0000
    .word  0x02028021
    .word  0x00701821
    .word  0x80620bb8
    .word  0x7bb00010
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cBEScoreInterface_getCurrentHighlightLevel
