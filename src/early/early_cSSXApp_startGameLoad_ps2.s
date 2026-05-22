.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cSSXApp_startGameLoad
.ent cSSXApp_startGameLoad
cSSXApp_startGameLoad:
    .word  0x27bdffe0
    .word  0x3c050048
    .word  0x7fb00010
    .word  0x0000302d
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x0000382d
    .word  0x24a5a808
    .word  0x0c0c5f5c
    .word  0x2404022c
    .word  0x0c08ba5a
    .word  0x0040202d
    .word  0x0200202d
    .word  0x0040282d
    .word  0x0c0c5bbe
    .word  0xae020084
    .word  0x0c08df3e
    .word  0x00000000
    .word  0xae0200c0
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cSSXApp_startGameLoad
