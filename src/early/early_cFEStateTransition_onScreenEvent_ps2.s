.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEStateTransition_onScreenEvent
.ent cFEStateTransition_onScreenEvent
cFEStateTransition_onScreenEvent:
    .word  0x27bdffb0
    .word  0x7fb10030
    .word  0x0080882d
    .word  0x7fb20020
    .word  0x7fb30010
    .word  0x3c04004a
    .word  0x24841360
    .word  0x7fb00040
    .word  0x00a0902d
    .word  0xffbf0000
    .word  0x0c0c5d9c
    .word  0x00c0982d
    .word  0x3c04004a
    .word  0x0040802d
    .word  0x0c0c5d9c
    .word  0x248417b8
    .word  0x16700009
    .word  0x2404ffbf
    .word  0x8e23001c
    .word  0x2402fff7
    .word  0x00621824
    .word  0xae23001c
    .word  0x8e420054
    .word  0x00441024
    .word  0x10000009
    .word  0xae420054
    .word  0x16620008
    .word  0x7bb00040
    .word  0x8e22001c
    .word  0x34420008
    .word  0xae22001c
    .word  0x8e430054
    .word  0x34630040
    .word  0xae430054
    .word  0x7bb00040
    .word  0x0000102d
    .word  0x7bb10030
    .word  0x7bb20020
    .word  0x7bb30010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0050
.end cFEStateTransition_onScreenEvent
