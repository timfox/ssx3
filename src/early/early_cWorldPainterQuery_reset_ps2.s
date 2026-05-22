.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldPainterQuery_reset
.ent cWorldPainterQuery_reset
cWorldPainterQuery_reset:
    .word  0x8f830790
    .word  0x1060000b
    .word  0x00000000
    .word  0xc780bd74
    .word  0x8c620000
    .word  0x00000000
    .word  0xe4400000
    .word  0x8c630014
    .word  0x00000000
    .word  0x00000000
    .word  0x00000000
    .word  0x5460fffa
    .word  0x8c620000
    .word  0x03e00008
    .word  0x00000000
    .word  0x00000000
.end cWorldPainterQuery_reset
