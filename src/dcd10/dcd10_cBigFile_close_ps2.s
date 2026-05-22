.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBigFile_close
.ent cBigFile_close
cBigFile_close:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0080802d
    .word  0x8e020004
    .word  0x10400005
    .word  0x24030001
    .word  0x8e040000
    .word  0x0c0f7b70
    .word  0x24050064
    .word  0x0002182b
    .word  0xae000004
    .word  0x0060102d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cBigFile_close
