.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEStateTransition_onCreateScreen
.ent cFEStateTransition_onCreateScreen
cFEStateTransition_onCreateScreen:
    .word  0x27bdffd0
    .word  0x7fb10010
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x7fb00020
    .word  0x3c040046
    .word  0x8e300010
    .word  0x0c0c5d9c
    .word  0x2484ffe8
    .word  0x0200202d
    .word  0x0220282d
    .word  0x0040302d
    .word  0x0c0e5f30
    .word  0x0000382d
    .word  0x3c040046
    .word  0x8e300010
    .word  0xae220048
    .word  0x0c0c5d9c
    .word  0x2484fff8
    .word  0x0200202d
    .word  0x0220282d
    .word  0x0040302d
    .word  0x0c0e5f30
    .word  0x0000382d
    .word  0xae22004c
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cFEStateTransition_onCreateScreen
