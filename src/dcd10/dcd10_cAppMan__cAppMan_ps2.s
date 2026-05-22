.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAppMan__cAppMan
.ent cAppMan__cAppMan
cAppMan__cAppMan:
    .word  0x27bdfff0
    .word  0x3c020049
    .word  0xffbf0000
    .word  0x2442dbe8
    .word  0xac82005c
    .word  0x30a50001
    .word  0x10a00003
    .word  0xaf802a74
    .word  0x0c0c5f94
    .word  0x00000000
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cAppMan__cAppMan
