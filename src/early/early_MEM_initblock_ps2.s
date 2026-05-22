.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global MEM_initblock
.ent MEM_initblock
MEM_initblock:
    .word  0x2402424d
    .word  0xac89000c
    .word  0xa4820000
    .word  0x24c20010
    .word  0xa4880002
    .word  0xac8a0008
    .word  0x03e00008
    .word  0xac860004
.end MEM_initblock
