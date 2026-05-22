.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global tModifierBlock_setBoundBox
.ent tModifierBlock_setBoundBox
tModifierBlock_setBoundBox:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0x8c860000
    .word  0x10c00007
    .word  0xdfbf0000
    .word  0x8cc20000
    .word  0x84440068
    .word  0x8c43006c
    .word  0x0060f809
    .word  0x00c42021
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end tModifierBlock_setBoundBox
