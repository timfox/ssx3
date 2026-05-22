.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cHandplantMotion_gainFocus
.ent cHandplantMotion_gainFocus
cHandplantMotion_gainFocus:
    .word  0x27bdffd0
    .word  0x8c8300a0
    .word  0xd8830030
    .word  0xd8640110
    .word  0x4be4196c
    .word  0xfba50020
    .word  0x7ba20020
    .word  0xc4800084
    .word  0x7fa20010
    .word  0x44040000
    .word  0x48a41800
    .word  0x4a6303bc
    .word  0xdba40010
    .word  0x4a0003bf
    .word  0x4be0211c
    .word  0xfba40020
    .word  0x7ba20020
    .word  0x7fa20000
    .word  0x7c6201e0
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cHandplantMotion_gainFocus
