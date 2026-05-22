.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBankInstance_OnAsyncMainMemAlloc
.ent cBankInstance_OnAsyncMainMemAlloc
cBankInstance_OnAsyncMainMemAlloc:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x8e020008
    .word  0x14400006
    .word  0x00a0882d
    .word  0x3c040048
    .word  0x0c094be8
    .word  0x248429a8
    .word  0xae020010
    .word  0xae110014
    .word  0x8e020010
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cBankInstance_OnAsyncMainMemAlloc
