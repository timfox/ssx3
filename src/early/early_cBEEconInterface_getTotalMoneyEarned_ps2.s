.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBEEconInterface_getTotalMoneyEarned
.ent cBEEconInterface_getTotalMoneyEarned
cBEEconInterface_getTotalMoneyEarned:
    .word  0x28a20002
    .word  0x1040000a
    .word  0x24030f88
    .word  0x34049b50
    .word  0x00c31818
    .word  0x00a42018
    .word  0x3c02004a
    .word  0x24426ca8
    .word  0x00641821
    .word  0x00621821
    .word  0x03e00008
    .word  0x8c620ac8
    .word  0x03e00008
    .word  0x0000102d
.end cBEEconInterface_getTotalMoneyEarned
