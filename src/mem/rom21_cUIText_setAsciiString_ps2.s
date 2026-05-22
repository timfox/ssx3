.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIText_setAsciiString
.ent cUIText_setAsciiString
cUIText_setAsciiString:
    .word  0x27bdfff0
    .word  0x0080102d
    .word  0xffbf0000
    .word  0x0c0e831c
    .word  0xac4000b0
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cUIText_setAsciiString
