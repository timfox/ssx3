.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIText_setUnicodeStringByID
.ent cUIText_setUnicodeStringByID
cUIText_setUnicodeStringByID:
    .word  0x27bdfff0
    .word  0xffbf0000
    .word  0xac8500b0
    .word  0x8c820008
    .word  0x0000282d
    .word  0x84430028
    .word  0x8c46002c
    .word  0x00c0f809
    .word  0x00832021
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cUIText_setUnicodeStringByID
