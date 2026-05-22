.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAppMan_cAppMan
.ent cAppMan_cAppMan
cAppMan_cAppMan:
    .word  0x3c020049
    .word  0x3c013f80
    .word  0x44810000
    .word  0xc781d104
    .word  0x2442dbe8
    .word  0x2403000c
    .word  0xac82005c
    .word  0xac830020
    .word  0x0080102d
    .word  0xe4800024
    .word  0xe4810030
    .word  0xac800034
    .word  0xac800000
    .word  0xac800004
    .word  0xac80001c
    .word  0xac800018
    .word  0xac800028
    .word  0xac80002c
    .word  0xac80000c
    .word  0x03e00008
    .word  0xaf842a74
    .word  0x00000000
.end cAppMan_cAppMan
