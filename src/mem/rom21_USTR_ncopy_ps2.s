.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global USTR_ncopy
.ent USTR_ncopy
USTR_ncopy:
    .word  0x18c0000e
    .word  0x0000382d
    .word  0x94a20000
    .word  0x1040000b
    .word  0x0080402d
    .word  0x94a30000
    .word  0x24e70001
    .word  0x24a50002
    .word  0x00e6102a
    .word  0xa5030000
    .word  0x10400004
    .word  0x25080002
    .word  0x94a20000
    .word  0x5440fff8
    .word  0x94a30000
    .word  0x00071040
    .word  0x00441021
    .word  0x03e00008
    .word  0xa4400000
    .word  0x00000000
.end USTR_ncopy
