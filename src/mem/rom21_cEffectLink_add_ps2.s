.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cEffectLink_add
.ent cEffectLink_add
cEffectLink_add:
    .word  0x8c820000
    .word  0x54400001
    .word  0xac450004
    .word  0xaca40004
    .word  0x8c820000
    .word  0xaca20000
    .word  0x03e00008
    .word  0xac850000
.end cEffectLink_add
