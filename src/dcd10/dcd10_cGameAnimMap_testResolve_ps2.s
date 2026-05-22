.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cGameAnimMap_testResolve
.ent cGameAnimMap_testResolve
cGameAnimMap_testResolve:
    .word  0x03e00008
    .word  0x00000000
.end cGameAnimMap_testResolve
