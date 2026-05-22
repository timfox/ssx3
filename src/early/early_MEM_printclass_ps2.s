.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global MEM_printclass
.ent MEM_printclass
MEM_printclass:
    .word  0x27bdfff0
    .word  0x3c050048
    .word  0x3c060025
    .word  0xffbf0000
    .word  0x24a500a8
    .word  0x0c094804
    .word  0x24c61f68
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end MEM_printclass
