.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMenuItem_cMenuItem
.ent cMenuItem_cMenuItem
cMenuItem_cMenuItem:
    .word  0x3c020048
    .word  0x24030001
    .word  0x24426f28
    .word  0xac830004
    .word  0xac820010
    .word  0x0080102d
    .word  0xac850008
    .word  0x03e00008
    .word  0xac80000c
    .word  0x00000000
.end cMenuItem_cMenuItem
