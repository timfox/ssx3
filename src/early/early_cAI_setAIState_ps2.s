.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAI_setAIState
.ent cAI_setAIState
cAI_setAIState:
    .word  0x27bdfff0
    .word  0x0080182d
    .word  0xffbf0000
    .word  0x8c660000
    .word  0x10a60006
    .word  0xdfbf0000
    .word  0x8c620098
    .word  0xac660004
    .word  0x0c044ed2
    .word  0xac62009c
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cAI_setAIState
