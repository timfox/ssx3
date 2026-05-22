.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global readTrigger
.ent readTrigger
readTrigger:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x0080802d
    .word  0x7fb20010
    .word  0x00a0882d
    .word  0xffbf0000
    .word  0x00c0902d
    .word  0x0c0b4956
    .word  0x8e250000
    .word  0x8e250000
    .word  0x0200202d
    .word  0x0c0b4956
    .word  0x24a50004
    .word  0x8e250000
    .word  0x0200202d
    .word  0x0240302d
    .word  0x0c05b132
    .word  0x24a50008
    .word  0x5440000e
    .word  0x7bb00030
    .word  0x8e250000
    .word  0x0200202d
    .word  0x0240302d
    .word  0x0c05b146
    .word  0x24a5000c
    .word  0x54400007
    .word  0x7bb00030
    .word  0x8e250000
    .word  0x0200202d
    .word  0x0240302d
    .word  0x0c05b146
    .word  0x24a50010
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
    .word  0x00000000
.end readTrigger
