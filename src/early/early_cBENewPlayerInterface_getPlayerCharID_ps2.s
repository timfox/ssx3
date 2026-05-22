.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewPlayerInterface_getPlayerCharID
.ent cBENewPlayerInterface_getPlayerCharID
cBENewPlayerInterface_getPlayerCharID:
    .word  0x2403001c
    .word  0x3c020053
    .word  0x00a32818
    .word  0x24424fe0
    .word  0x00a22821
    .word  0x03e00008
    .word  0x80a20011
    .word  0x00000000
.end cBENewPlayerInterface_getPlayerCharID
