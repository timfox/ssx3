.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global tModifierBlock_setRadius
.ent tModifierBlock_setRadius
tModifierBlock_setRadius:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0x8c850000
    .word  0x10a00007
    .word  0xdfbf0000
    .word  0x8ca20000
    .word  0x84440078
    .word  0x8c43007c
    .word  0x0060f809
    .word  0x00a42021
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end tModifierBlock_setRadius
