.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAnimSequence_fadeWeight
.ent cAnimSequence_fadeWeight
cAnimSequence_fadeWeight:
    .word  0xe48c0098
    .word  0xe48d009c
    .word  0x03e00008
    .word  0xac8000a0
.end cAnimSequence_fadeWeight
