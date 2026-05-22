.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cExecutionMan_prepause
.ent cExecutionMan_prepause
cExecutionMan_prepause:
    .word  0x03e00008
    .word  0x00000000
.end cExecutionMan_prepause
