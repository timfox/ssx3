.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplay_stopAutoReplay
.ent cReplay_stopAutoReplay
cReplay_stopAutoReplay:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0080802d
    .word  0x8e02061c
    .word  0x50400004
    .word  0x0200202d
    .word  0x0c09be60
    .word  0x00000000
    .word  0x0200202d
    .word  0x0c09bd28
    .word  0x2405000d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cReplay_stopAutoReplay
