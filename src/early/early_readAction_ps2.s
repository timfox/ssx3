.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global readAction
.ent readAction
readAction:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x0080802d
    .word  0x7fb20010
    .word  0x00a0882d
    .word  0xffbf0000
    .word  0x0c05c4ac
    .word  0x00c0902d
    .word  0x54400003
    .word  0x8e250000
    .word  0x10000011
    .word  0x24020006
    .word  0x24020001
    .word  0x8ca30000
    .word  0x14620007
    .word  0x24020002
    .word  0x0200202d
    .word  0x24a50008
    .word  0x0c05b13c
    .word  0x0240302d
    .word  0x10000008
    .word  0x7bb00030
    .word  0x14620005
    .word  0x0000102d
    .word  0x0200202d
    .word  0x0c05c756
    .word  0x24a50008
    .word  0x0000102d
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
    .word  0x00000000
.end readAction
