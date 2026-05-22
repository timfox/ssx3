.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldView_getNumSections
.ent cWorldView_getNumSections
cWorldView_getNumSections:
    .word  0x8c820004
    .word  0x10400003
    .word  0x00000000
    .word  0x03e00008
    .word  0x8c420008
    .word  0x03e00008
    .word  0x0000102d
    .word  0x00000000
.end cWorldView_getNumSections
