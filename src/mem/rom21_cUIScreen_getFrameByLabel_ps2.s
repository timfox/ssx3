.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_getFrameByLabel
.ent cUIScreen_getFrameByLabel
cUIScreen_getFrameByLabel:
    .word  0x8c820034
    .word  0x0000302d
    .word  0x8c430000
    .word  0x1060000b
    .word  0x24440004
    .word  0x0060382d
    .word  0x8c820000
    .word  0x54450003
    .word  0x8c830004
    .word  0x03e00008
    .word  0x94820008
    .word  0x24c60001
    .word  0x00c7102b
    .word  0x1440fff8
    .word  0x00832021
    .word  0x03e00008
    .word  0x3402ffff
    .word  0x00000000
.end cUIScreen_getFrameByLabel
