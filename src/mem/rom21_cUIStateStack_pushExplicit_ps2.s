.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIStateStack_pushExplicit
.ent cUIStateStack_pushExplicit
cUIStateStack_pushExplicit:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x00a0802d
    .word  0xffbf0000
    .word  0x0080882d
    .word  0x8e02001c
    .word  0x8e030008
    .word  0x34420060
    .word  0xae02001c
    .word  0x84640020
    .word  0x8c620024
    .word  0x0040f809
    .word  0x02042021
    .word  0x2624001c
    .word  0x0c0e5e44
    .word  0x0200282d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cUIStateStack_pushExplicit
