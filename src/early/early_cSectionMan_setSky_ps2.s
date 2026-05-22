.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cSectionMan_setSky
.ent cSectionMan_setSky
cSectionMan_setSky:
    .word  0x27bdfff0
    .word  0x2ca20016
    .word  0x1040001c
    .word  0xffbf0000
    .word  0x3c020048
    .word  0x00051880
    .word  0x2442b4e0
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cSectionMan_setSky
