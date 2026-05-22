.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cExecutionMan_halt
.ent cExecutionMan_halt
cExecutionMan_halt:
    .word  0x3c030049
    .word  0x0080102d
    .word  0x2463dc30
    .word  0xac800000
    .word  0xac830008
    .word  0x03e00008
    .word  0xac800004
    .word  0x00000000
.end cExecutionMan_halt
