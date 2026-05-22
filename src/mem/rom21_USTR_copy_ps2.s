.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global USTR_copy
.ent USTR_copy
USTR_copy:
    .word  0x27bdfff0
    .word  0x3c067fff
    .word  0xffbf0000
    .word  0x0c0f6f90
    .word  0x34c6ffff
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end USTR_copy
