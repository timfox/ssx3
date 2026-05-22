.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerMan_streamIn
.ent cCameraTriggerMan_streamIn
cCameraTriggerMan_streamIn:
    .word  0x27bdffd0
    .word  0x7fb10010
    .word  0x7fb00020
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x0c05b26c
    .word  0x00a0802d
    .word  0x0200282d
    .word  0x0c05b08a
    .word  0x0220202d
    .word  0x8e230004
    .word  0x1860000b
    .word  0x0000282d
    .word  0x8e240000
    .word  0x00051080
    .word  0x24a50001
    .word  0x00441021
    .word  0x8c430000
    .word  0xac600020
    .word  0x8e220004
    .word  0x00a2102a
    .word  0x5440fff8
    .word  0x8e240000
    .word  0x0c05b46e
    .word  0x0220202d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cCameraTriggerMan_streamIn
