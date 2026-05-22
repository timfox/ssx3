.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBigFile_cBigFile
.ent cBigFile_cBigFile
cBigFile_cBigFile:
    .word  0x2402ffff
    .word  0xac800004
    .word  0xac820000
    .word  0x03e00008
    .word  0x0080102d
    .word  0x00000000
.end cBigFile_cBigFile
