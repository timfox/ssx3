.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIThing_getKeyframerEvent
.ent cUIThing_getKeyframerEvent
cUIThing_getKeyframerEvent:
    .word  0x00052e00
    .word  0x8c82003c
    .word  0x14400003
    .word  0x00051e03
    .word  0x03e00008
    .word  0x3402ffff
    .word  0x00031840
    .word  0x00621821
    .word  0x03e00008
    .word  0x94620000
.end cUIThing_getKeyframerEvent
