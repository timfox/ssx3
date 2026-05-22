.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_getPrimaryThread
.ent cUIScreen_getPrimaryThread
cUIScreen_getPrimaryThread:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c0e5e10
    .word  0x24840018
    .word  0x0040802d
    .word  0x1200000d
    .word  0x0000102d
    .word  0x10000007
    .word  0x00000000
    .word  0x8e020010
    .word  0x00021643
    .word  0x30420001
    .word  0x14400006
    .word  0x0200102d
    .word  0x8e100004
    .word  0x0c0e5dc6
    .word  0x0200202d
    .word  0x1040fff7
    .word  0x0000102d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cUIScreen_getPrimaryThread
