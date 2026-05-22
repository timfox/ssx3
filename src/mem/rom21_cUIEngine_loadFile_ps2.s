.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIEngine_loadFile
.ent cUIEngine_loadFile
cUIEngine_loadFile:
    .word  0x27bdff60
    .word  0x7fb10080
    .word  0x7fb00090
    .word  0x00a0882d
    .word  0x0080802d
    .word  0x3c05004a
    .word  0xffbf0070
    .word  0x03a0202d
    .word  0x0220302d
    .word  0x0c105e0a
    .word  0x24a54770
    .word  0x03a0202d
    .word  0x0c0f8632
    .word  0x0000282d
    .word  0xae020004
    .word  0x26040050
    .word  0x8c450008
    .word  0x0c0e5c4e
    .word  0x00452821
    .word  0x8e020004
    .word  0x26040058
    .word  0x0220282d
    .word  0x8c460010
    .word  0x0c0e604e
    .word  0x00463021
    .word  0x7bb00090
    .word  0x24020001
    .word  0x7bb10080
    .word  0xdfbf0070
    .word  0x03e00008
    .word  0x27bd00a0
    .word  0x00000000
.end cUIEngine_loadFile
