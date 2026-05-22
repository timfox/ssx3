.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIThread_deleteThread
.ent cUIThread_deleteThread
cUIThread_deleteThread:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c0e5dd0
    .word  0x0080802d
    .word  0x12000006
    .word  0x24050003
    .word  0x8e020008
    .word  0x84440008
    .word  0x8c43000c
    .word  0x0060f809
    .word  0x02042021
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cUIThread_deleteThread
