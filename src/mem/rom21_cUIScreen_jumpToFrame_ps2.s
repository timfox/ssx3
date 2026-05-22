.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_jumpToFrame
.ent cUIScreen_jumpToFrame
cUIScreen_jumpToFrame:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x0080802d
    .word  0x7fb10010
    .word  0xffbf0000
    .word  0x0c0e71f0
    .word  0x30b1ffff
    .word  0x10400004
    .word  0x0200202d
    .word  0x0c0e7146
    .word  0x0040202d
    .word  0x0200202d
    .word  0x0220282d
    .word  0x0c0e721c
    .word  0x0000302d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cUIScreen_jumpToFrame
