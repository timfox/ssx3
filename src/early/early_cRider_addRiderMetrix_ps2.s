.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_addRiderMetrix
.ent cRider_addRiderMetrix
cRider_addRiderMetrix:
    .word  0x27bdffe0
    .word  0x3c050045
    .word  0x7fb00010
    .word  0x24a57970
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x0000302d
    .word  0x0000382d
    .word  0x0c0c5f5c
    .word  0x240401cc
    .word  0x0c045c92
    .word  0x0040202d
    .word  0x0200282d
    .word  0xae020790
    .word  0x0c045cb8
    .word  0x0040202d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cRider_addRiderMetrix
