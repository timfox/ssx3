.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cSSXApp_flush
.ent cSSXApp_flush
cSSXApp_flush:
    .word  0x27bdfff0
    .word  0x8f84f7b0
    .word  0x14800003
    .word  0xffbf0000
    .word  0x10000003
    .word  0x0000102d
    .word  0x0c0c9b18
    .word  0x00000000
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cSSXApp_flush
