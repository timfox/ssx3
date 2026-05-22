.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_initOnce
.ent cRider_initOnce
cRider_initOnce:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0x10a00005
    .word  0xffbf0000
    .word  0x0c0470a6
    .word  0x00000000
    .word  0x1000000d
    .word  0x7bb00020
    .word  0x8e030880
    .word  0x24020007
    .word  0x10620008
    .word  0x24110007
    .word  0x0c0470a6
    .word  0x0200202d
    .word  0x0c0f94ac
    .word  0x24040001
    .word  0x8e020880
    .word  0x1451fffa
    .word  0x00000000
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cRider_initOnce
