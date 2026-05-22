.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAppMan_checkHalt
.ent cAppMan_checkHalt
cAppMan_checkHalt:
    .word  0x27bdfff0
    .word  0x24050001
    .word  0xffbf0000
    .word  0x0c0c5d2a
    .word  0x8c840008
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end cAppMan_checkHalt
