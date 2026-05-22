.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBaseClass_DynamicEmitter_reset
.ent cBaseClass_DynamicEmitter_reset
cBaseClass_DynamicEmitter_reset:
    .word  0x0080302d
    .word  0x0000382d
    .word  0x8cc20178
    .word  0x18400013
    .word  0xacc0017c
    .word  0x3c020050
    .word  0x2448f120
    .word  0x00000000
    .word  0x8cc301a0
    .word  0x00072900
    .word  0x79020000
    .word  0x24e70001
    .word  0x00a31821
    .word  0x7c620000
    .word  0x8cc401a4
    .word  0x79020000
    .word  0x00a42821
    .word  0x7ca20000
    .word  0xacc0017c
    .word  0x8cc20178
    .word  0x00e2102a
    .word  0x1440fff2
    .word  0xacc001e0
    .word  0x03e00008
    .word  0x00000000
    .word  0x00000000
.end cBaseClass_DynamicEmitter_reset
