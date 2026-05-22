.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWorldView_isSectionLoaded
.ent cWorldView_isSectionLoaded
cWorldView_isSectionLoaded:
    .word  0x8c820004
    .word  0x10400005
    .word  0x00000000
    .word  0x8c420008
    .word  0x00a2102b
    .word  0x14400003
    .word  0x000518c0
    .word  0x03e00008
    .word  0x0000102d
    .word  0x00831821
    .word  0x8c620014
    .word  0x2442fffb
    .word  0x03e00008
    .word  0x2c420002
.end cWorldView_isSectionLoaded
