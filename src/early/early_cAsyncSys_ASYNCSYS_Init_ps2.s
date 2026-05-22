.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAsyncSys_ASYNCSYS_Init
.ent cAsyncSys_ASYNCSYS_Init
cAsyncSys_ASYNCSYS_Init:
    .word  0x27bdffe0
    .word  0x00a0102d
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x10400008
    .word  0x0080802d
    .word  0x3c050048
    .word  0x0040202d
    .word  0x24a52988
    .word  0xae0201d0
    .word  0x0c0c5f8c
    .word  0x0000382d
    .word  0xae0201cc
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end cAsyncSys_ASYNCSYS_Init
