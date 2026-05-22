.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global USTR_length
.ent USTR_length
USTR_length:
    .word  0x94820000
    .word  0x10400009
    .word  0x0000182d
    .word  0x00000000
    .word  0x24840002
    .word  0x94820000
    .word  0x00000000
    .word  0x00000000
    .word  0x00000000
    .word  0x1440fffa
    .word  0x24630001
    .word  0x03e00008
    .word  0x0060102d
    .word  0x00000000
.end USTR_length
