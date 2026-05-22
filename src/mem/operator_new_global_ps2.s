.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global operator_new
.ent operator_new
operator_new:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0x0c0c5f5c
    .word  0x00000000
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end operator_new
