.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global SSXAUDIO_Init
.ent SSXAUDIO_Init
SSXAUDIO_Init:
    .word  0x27bdffe0
    .word  0x8f820410
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x1440000f
    .word  0x0080802d
    .word  0x3c050048
    .word  0x3c068000
    .word  0x24a528b0
    .word  0x24047780
    .word  0x34c60400
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0040202d
    .word  0x0200402d
    .word  0x24050001
    .word  0x2406001c
    .word  0x0c0a131a
    .word  0x24070003
    .word  0xaf820410
    .word  0x8f820410
    .word  0x8c430118
    .word  0x8c640000
    .word  0x0c0ab6fc
    .word  0x248401d8
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end SSXAUDIO_Init
