.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIText_deleteText
.ent cUIText_deleteText
cUIText_deleteText:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0080802d
    .word  0xae0000a8
    .word  0x8e0400ac
    .word  0x50800005
    .word  0x8e0400b4
    .word  0x0c0c5fa6
    .word  0x00000000
    .word  0xae0000ac
    .word  0x8e0400b4
    .word  0x50800005
    .word  0x7bb00010
    .word  0x0c0c5fa6
    .word  0x00000000
    .word  0xae0000b4
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cUIText_deleteText
