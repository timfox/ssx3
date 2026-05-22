.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_cCTActionSwitchCam
.ent get_cCTActionSwitchCam
get_cCTActionSwitchCam:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x00a0802d
    .word  0x7fb20010
    .word  0x0080882d
    .word  0xffbf0000
    .word  0xae000000
    .word  0x0c0b4944
    .word  0x2605000c
    .word  0x0040902d
    .word  0x0220202d
    .word  0x0c0b4956
    .word  0x26050008
    .word  0x02421021
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
    .word  0x00000000
.end get_cCTActionSwitchCam
