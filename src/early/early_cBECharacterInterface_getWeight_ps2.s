.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBECharacterInterface_getWeight
.ent cBECharacterInterface_getWeight
cBECharacterInterface_getWeight:
    .word  0x27bdffe0
    .word  0x00a0202d
    .word  0x7fb00010
    .word  0x3c100053
    .word  0xffbf0000
    .word  0x0c052820
    .word  0x26100970
    .word  0x24030088
    .word  0xdfbf0000
    .word  0x00431018
    .word  0x02028021
    .word  0x8e020040
    .word  0x7bb00010
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cBECharacterInterface_getWeight
