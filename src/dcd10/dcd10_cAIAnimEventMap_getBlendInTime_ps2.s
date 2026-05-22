.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAIAnimEventMap_getBlendInTime
.ent cAIAnimEventMap_getBlendInTime
cAIAnimEventMap_getBlendInTime:
    .word  0x2403001c
    .word  0x3c020044
    .word  0x00832018
    .word  0x24426990
    .word  0x03e00008
    .word  0x00821021
.end cAIAnimEventMap_getBlendInTime
