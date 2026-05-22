.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_float
.ent get_float
get_float:
    .word  0x8c860000
    .word  0x24020004
    .word  0x90c30003
    .word  0x90c70002
    .word  0x90c80001
    .word  0x00031e00
    .word  0x00073c00
    .word  0x90c90000
    .word  0x00671825
    .word  0x00084200
    .word  0x00681825
    .word  0x00693825
    .word  0xaca70000
    .word  0x8c830000
    .word  0x24630004
    .word  0x03e00008
    .word  0xac830000
    .word  0x00000000
.end get_float
