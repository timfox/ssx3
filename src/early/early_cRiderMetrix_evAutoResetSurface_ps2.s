.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRiderMetrix_evAutoResetSurface
.ent cRiderMetrix_evAutoResetSurface
cRiderMetrix_evAutoResetSurface:
    .word  0x27bdfff0
    .word  0x3c013fc0
    .word  0x44816000
    .word  0x3c060045
    .word  0xffbf0000
    .word  0x24c67888
    .word  0x24050033
    .word  0x0c045eba
    .word  0x0000382d
    .word  0x44800000
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cRiderMetrix_evAutoResetSurface
