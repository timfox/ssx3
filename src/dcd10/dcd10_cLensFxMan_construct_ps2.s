.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cLensFxMan_construct
.ent cLensFxMan_construct
cLensFxMan_construct:
    .word  0x27bdfff0
    .word  0x3c05004a
    .word  0xffbf0000
    .word  0x24a53b48
    .word  0x0000302d
    .word  0x0000382d
    .word  0x0c0c5f5c
    .word  0x24046820
    .word  0x0c0bb106
    .word  0x0040202d
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cLensFxMan_construct
