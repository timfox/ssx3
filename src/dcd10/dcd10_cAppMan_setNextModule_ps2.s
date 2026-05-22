.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAppMan_setNextModule
.ent cAppMan_setNextModule
cAppMan_setNextModule:
    .word  0x03e00008
    .word  0xac850004
.end cAppMan_setNextModule
