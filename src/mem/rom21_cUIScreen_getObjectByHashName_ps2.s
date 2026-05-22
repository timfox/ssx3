.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_getObjectByHashName
.ent cUIScreen_getObjectByHashName
cUIScreen_getObjectByHashName:
    .word  0x8c83003c
    .word  0x10600013
    .word  0x00000000
    .word  0x8c820038
    .word  0x8c420000
    .word  0x1040000f
    .word  0x0000382d
    .word  0x0040202d
    .word  0x0060302d
    .word  0x00000000
    .word  0x8cc30000
    .word  0x10600006
    .word  0x24e70001
    .word  0x8c620038
    .word  0x14450004
    .word  0x00e4102b
    .word  0x03e00008
    .word  0x0060102d
    .word  0x00e4102b
    .word  0x1440fff6
    .word  0x24c60004
    .word  0x03e00008
    .word  0x0000102d
    .word  0x00000000
.end cUIScreen_getObjectByHashName
