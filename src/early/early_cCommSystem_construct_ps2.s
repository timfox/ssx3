.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCommSystem_construct
.ent cCommSystem_construct
cCommSystem_construct:
    .word  0x27bdfff0
    .word  0x3c050048
    .word  0xffbf0000
    .word  0x24a50300
    .word  0x3c062000
    .word  0x0000382d
    .word  0x0c0c5f5c
    .word  0x24040034
    .word  0x0c095630
    .word  0x0040202d
    .word  0xdfbf0000
    .word  0xaf82fdc8
    .word  0x03e00008
    .word  0x27bd0010
.end cCommSystem_construct
