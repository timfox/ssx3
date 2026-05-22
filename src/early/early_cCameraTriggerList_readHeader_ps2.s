.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerList_readHeader
.ent cCameraTriggerList_readHeader
cCameraTriggerList_readHeader:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x00a0802d
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x0200202d
    .word  0x0c0b4956
    .word  0x26250010
    .word  0x0200202d
    .word  0x0c0b4956
    .word  0x26250004
    .word  0x0200202d
    .word  0x0c0b4956
    .word  0x26250008
    .word  0x7bb00020
    .word  0x0000102d
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cCameraTriggerList_readHeader
