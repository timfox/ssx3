.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldMemoryMan_activateSectionMem
.ent cWorldMemoryMan_activateSectionMem
cWorldMemoryMan_activateSectionMem:
    .word  0x27bdffb0
    .word  0x0000382d
    .word  0x7fb10030
    .word  0x7fb20020
    .word  0x0080882d
    .word  0x00a0902d
    .word  0x7fb00040
    .word  0x7fb30010
    .word  0x00121080
    .word  0x00c0982d
    .word  0x2630003c
    .word  0x3c050049
    .word  0xffbf0000
    .word  0x02028021
    .word  0x24a54de8
    .word  0x24040014
    .word  0x0c0c5f5c
    .word  0x3c062000
    .word  0x0040202d
    .word  0x24050014
    .word  0x02651018
    .word  0x0240302d
    .word  0x0c0e9dec
    .word  0x00512821
    .word  0xae020000
    .word  0x7bb00040
    .word  0x7bb10030
    .word  0x7bb20020
    .word  0x7bb30010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0050
.end cWorldMemoryMan_activateSectionMem
