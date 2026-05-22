.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global tModifierBlock_tModifierBlock
.ent tModifierBlock_tModifierBlock
tModifierBlock_tModifierBlock:
    .word  0x3c020049
    .word  0x3c030049
    .word  0x24421340
    .word  0x24631200
    .word  0xac820018
    .word  0xac830024
    .word  0x0080102d
    .word  0xac800010
    .word  0xac800014
    .word  0xac80001c
    .word  0xac800020
    .word  0xac800000
    .word  0xac800004
    .word  0xac800008
    .word  0x03e00008
    .word  0xac80000c
.end tModifierBlock_tModifierBlock
