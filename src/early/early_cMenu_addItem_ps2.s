.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMenu_addItem
.ent cMenu_addItem
cMenu_addItem:
    .word  0x04c10002
    .word  0xaca40000
    .word  0x8c860000
    .word  0x8c880000
    .word  0x2489000c
    .word  0x00c8102a
    .word  0x1040000a
    .word  0x00065080
    .word  0x00081080
    .word  0x00493821
    .word  0x8ce2fffc
    .word  0x2508ffff
    .word  0x00c8182a
    .word  0xace20000
    .word  0x00000000
    .word  0x1460fffa
    .word  0x24e7fffc
    .word  0x012a1021
    .word  0xac450000
    .word  0x8c830124
    .word  0x50600009
    .word  0x8c820000
    .word  0x8c830004
    .word  0x0066102a
    .word  0x14400005
    .word  0x8c820000
    .word  0x10400002
    .word  0x24620001
    .word  0xac820004
    .word  0x8c820000
    .word  0x24420001
    .word  0x03e00008
    .word  0xac820000
    .word  0x00000000
.end cMenu_addItem
