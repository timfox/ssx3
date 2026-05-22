.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEFlowState_setNumStates
.ent cFEFlowState_setNumStates
cFEFlowState_setNumStates:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x00058600
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x0c0681ac
    .word  0x00108603
    .word  0xa2300000
    .word  0x3c050046
    .word  0x0200202d
    .word  0x0000382d
    .word  0x24a50b80
    .word  0x0c0c5f8c
    .word  0x24060100
    .word  0x0200302d
    .word  0xae220004
    .word  0x0040202d
    .word  0x0c105884
    .word  0x0000282d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cFEFlowState_setNumStates
