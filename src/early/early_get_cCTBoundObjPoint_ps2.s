.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_cCTBoundObjPoint
.ent get_cCTBoundObjPoint
get_cCTBoundObjPoint:
    .word  0x27bdffe0
    .word  0x24020003
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0080802d
    .word  0xaca20028
    .word  0x8ca30024
    .word  0x84640008
    .word  0x8c62000c
    .word  0x0040f809
    .word  0x00a42021
    .word  0x0200202d
    .word  0x0c0b4968
    .word  0x0040282d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end get_cCTBoundObjPoint
