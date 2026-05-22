.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_uint
.ent get_uint
get_uint:
    .word  0x8c830000
    .word  0x24020004
    .word  0x90660003
    .word  0x90670002
    .word  0x90680001
    .word  0x00063600
    .word  0x00073c00
    .word  0x90690000
    .word  0x00c73025
    .word  0x00084200
    .word  0x00c83025
    .word  0x00c93025
    .word  0xaca60000
    .word  0x8c830000
    .word  0x24630004
    .word  0x03e00008
    .word  0xac830000
    .word  0x00000000
.end get_uint
