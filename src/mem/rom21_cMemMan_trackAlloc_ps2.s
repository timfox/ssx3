.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cMemMan_trackAlloc
.ent cMemMan_trackAlloc
cMemMan_trackAlloc:
    .word  0x27bdfff0
    .word  0x00a0302d
    .word  0xffbf0000
    .word  0x0c0f9912
    .word  0x0000282d
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cMemMan_trackAlloc
