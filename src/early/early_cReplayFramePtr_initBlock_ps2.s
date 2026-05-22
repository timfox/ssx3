.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplayFramePtr_initBlock
.ent cReplayFramePtr_initBlock
cReplayFramePtr_initBlock:
    .word  0x27bdffe0
    .word  0x3c020048
    .word  0x7fb00010
    .word  0x0000302d
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x00a0202d
    .word  0x0000382d
    .word  0x0c0c5f8c
    .word  0x24451730
    .word  0xae000010
    .word  0xae02000c
    .word  0xae020004
    .word  0xae020008
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cReplayFramePtr_initBlock
