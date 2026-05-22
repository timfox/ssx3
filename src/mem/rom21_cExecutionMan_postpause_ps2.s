.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cExecutionMan_postpause
.ent cExecutionMan_postpause
cExecutionMan_postpause:
    .word  0x03e00008
    .word  0x00000000
.end cExecutionMan_postpause
