.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplay_restoreObject
.ent cReplay_restoreObject
cReplay_restoreObject:
    .word  0x27bdffd0
    .word  0x7fb10010
    .word  0x7fb00020
    .word  0x00c0882d
    .word  0xffbf0000
    .word  0x00a0802d
    .word  0x0c09ba9a
    .word  0x0220202d
    .word  0x0040182d
    .word  0x2c620017
    .word  0x10400109
    .word  0x3c020048
    .word  0x00031880
    .word  0x244216c0
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cReplay_restoreObject
