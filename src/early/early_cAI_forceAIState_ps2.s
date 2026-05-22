.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAI_forceAIState
.ent cAI_forceAIState
cAI_forceAIState:
    .word  0x24a3ffff
    .word  0x2c620007
    .word  0x1040002e
    .word  0x0080302d
    .word  0x3c020045
    .word  0x00031880
    .word  0x24426ca0
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cAI_forceAIState
