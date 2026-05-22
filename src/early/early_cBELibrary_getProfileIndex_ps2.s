.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBELibrary_getProfileIndex
.ent cBELibrary_getProfileIndex
cBELibrary_getProfileIndex:
    .word  0x3c020053
    .word  0x00042080
    .word  0x244205b0
    .word  0x2403001c
    .word  0x00822021
    .word  0x3c050053
    .word  0x8c820000
    .word  0x24a55b20
    .word  0x2404ffff
    .word  0x00431018
    .word  0x00451021
    .word  0x8c43000c
    .word  0x10640004
    .word  0x00000000
    .word  0x8c420010
    .word  0x03e00008
    .word  0x30420001
    .word  0x03e00008
    .word  0x24020002
    .word  0x00000000
.end cBELibrary_getProfileIndex
