.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMCOverlayManager_getManager
.ent cMCOverlayManager_getManager
cMCOverlayManager_getManager:
    .word  0x27bdfff0
    .word  0x8f82fb84
    .word  0x1440000a
    .word  0xffbf0000
    .word  0x3c050048
    .word  0x2404074c
    .word  0x24a5c128
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0c0908a2
    .word  0x0040202d
    .word  0xaf82fb84
    .word  0x8f82fb84
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cMCOverlayManager_getManager
