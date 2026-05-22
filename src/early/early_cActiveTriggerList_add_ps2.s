.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cActiveTriggerList_add
.ent cActiveTriggerList_add
cActiveTriggerList_add:
    .word  0x27bdffd0
    .word  0x3c062000
    .word  0x7fb10010
    .word  0x0000382d
    .word  0x00a0882d
    .word  0x7fb00020
    .word  0x0080802d
    .word  0x3c050046
    .word  0xffbf0000
    .word  0x24040010
    .word  0x0c0c5f5c
    .word  0x24a5bf68
    .word  0x8e040000
    .word  0x24030001
    .word  0xae020000
    .word  0xac510000
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0xac430008
    .word  0xac440004
    .word  0xac43000c
    .word  0x03e00008
    .word  0x27bd0030
.end cActiveTriggerList_add
