.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAnimSequencer_getSequence
.ent cAnimSequencer_getSequence
cAnimSequencer_getSequence:
    .word  0x10000002
    .word  0x8c830004
    .word  0x8c6300c8
    .word  0x10600003
    .word  0x00a0102d
    .word  0x1c40fffc
    .word  0x24a5ffff
    .word  0x03e00008
    .word  0x0060102d
    .word  0x00000000
.end cAnimSequencer_getSequence
