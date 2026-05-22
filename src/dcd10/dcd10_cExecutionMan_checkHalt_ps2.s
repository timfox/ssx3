.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cExecutionMan_checkHalt
.ent cExecutionMan_checkHalt
cExecutionMan_checkHalt:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x8e020000
    .word  0x1040000a
    .word  0x00a0882d
    .word  0x0040f809
    .word  0x8e040004
    .word  0x52200007
    .word  0x7bb00020
    .word  0x8e020008
    .word  0x84440070
    .word  0x8c430074
    .word  0x0060f809
    .word  0x02042021
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cExecutionMan_checkHalt
