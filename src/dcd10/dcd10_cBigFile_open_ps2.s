.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBigFile_open
.ent cBigFile_open
cBigFile_open:
    .word  0x27bdffb0
    .word  0x7fb00040
    .word  0x7fb10030
    .word  0x0080802d
    .word  0x00a0882d
    .word  0x7fb20020
    .word  0x7fb30010
    .word  0x00c0902d
    .word  0xffbf0000
    .word  0x0220202d
    .word  0x0c0f7b86
    .word  0x24050064
    .word  0x14400003
    .word  0x8f93fd88
    .word  0x1000000c
    .word  0x0000102d
    .word  0x0220202d
    .word  0x0240282d
    .word  0xaf92fd88
    .word  0x24060064
    .word  0x0c0f7b54
    .word  0x0200382d
    .word  0x10400002
    .word  0x24020001
    .word  0xae020004
    .word  0xaf93fd88
    .word  0x8e020004
    .word  0x7bb00040
    .word  0x7bb10030
    .word  0x7bb20020
    .word  0x7bb30010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0050
.end cBigFile_open
