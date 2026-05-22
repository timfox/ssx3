.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewPlayerInterface_getPlayerID
.ent cBENewPlayerInterface_getPlayerID
cBENewPlayerInterface_getPlayerID:
    .word  0x3c020053
    .word  0x00042080
    .word  0x244205b0
    .word  0x00822021
    .word  0x03e00008
    .word  0x8c820000
.end cBENewPlayerInterface_getPlayerID
