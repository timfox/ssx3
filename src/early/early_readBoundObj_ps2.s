.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global readBoundObj
.ent readBoundObj
readBoundObj:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0x0c05c5a4
    .word  0x00000000
    .word  0x24030005
    .word  0xdfbf0000
    .word  0x0002180b
    .word  0x0060102d
    .word  0x03e00008
    .word  0x27bd0010
.end readBoundObj
