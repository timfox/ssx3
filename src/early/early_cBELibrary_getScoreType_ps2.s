.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBELibrary_getScoreType
.ent cBELibrary_getScoreType
cBELibrary_getScoreType:
    .word  0x28820011
    .word  0x14400003
    .word  0x24a6fffa
    .word  0x03e00008
    .word  0x2402001a
    .word  0x2cc20003
    .word  0x14400004
    .word  0x24a2fff7
    .word  0x2c420003
    .word  0x10400005
    .word  0x3c020046
    .word  0x03e00008
    .word  0x00c0102d
    .word  0x03e00008
    .word  0x8c620004
    .word  0x00041900
    .word  0x2442a2f8
    .word  0x0000382d
    .word  0x00621821
    .word  0x0060302d
    .word  0x8cc20000
    .word  0x1045fff7
    .word  0x24e70001
    .word  0x24c60008
    .word  0x28e20002
    .word  0x1440fffa
    .word  0x24630008
    .word  0x03e00008
    .word  0x2402001a
    .word  0x00000000
.end cBELibrary_getScoreType
