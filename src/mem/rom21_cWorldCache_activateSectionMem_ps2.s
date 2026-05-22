.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldCache_activateSectionMem
.ent cWorldCache_activateSectionMem
cWorldCache_activateSectionMem:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x00a0802d
    .word  0x7fb20010
    .word  0xffbf0000
    .word  0x0080902d
    .word  0x0c0e9ec6
    .word  0x8e44000c
    .word  0x8e420008
    .word  0x00108880
    .word  0x3c050049
    .word  0x0000382d
    .word  0x02228821
    .word  0x24a54e60
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x240400e8
    .word  0x24030058
    .word  0x8e460000
    .word  0x02038018
    .word  0x0040202d
    .word  0x8cc50014
    .word  0x00b02821
    .word  0x0c0e9f22
    .word  0x24a50020
    .word  0xae220000
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
    .word  0x00000000
.end cWorldCache_activateSectionMem
