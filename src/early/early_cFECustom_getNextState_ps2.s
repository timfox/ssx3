.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFECustom_getNextState
.ent cFECustom_getNextState
cFECustom_getNextState:
    .word  0x27bdff90
    .word  0x7fb00060
    .word  0x7fb10050
    .word  0x00a0802d
    .word  0x7fb20040
    .word  0x00c0882d
    .word  0x7fb30030
    .word  0x7fb40020
    .word  0x7fb50010
    .word  0xffbf0000
    .word  0x8e03000c
    .word  0x2c62003c
    .word  0x1040038c
    .word  0x0080902d
    .word  0x3c020046
    .word  0x00031880
    .word  0x24420f30
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cFECustom_getNextState
