.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerMan_cleanupOnExit
.ent cCameraTriggerMan_cleanupOnExit
cCameraTriggerMan_cleanupOnExit:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x0c05b18a
    .word  0x26040014
    .word  0x0c05b18a
    .word  0x26040018
    .word  0x0c05b480
    .word  0x2604001c
    .word  0x0c05b480
    .word  0x2604003c
    .word  0x0c05b2f8
    .word  0x0200202d
    .word  0x0c05afc8
    .word  0x0200202d
    .word  0x0c057706
    .word  0x00000000
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cCameraTriggerMan_cleanupOnExit
