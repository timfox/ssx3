.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global MUTEX_unlock
.ent MUTEX_unlock
MUTEX_unlock:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c108f24
    .word  0x0080802d
    .word  0x8e030004
    .word  0x14620007
    .word  0x3c020045
    .word  0x8e020008
    .word  0x2442ffff
    .word  0x1440000c
    .word  0xae020008
    .word  0xae000004
    .word  0x3c020045
    .word  0x8c430de4
    .word  0x10600005
    .word  0x00000000
    .word  0x0c108f74
    .word  0x8e04000c
    .word  0x10000004
    .word  0x7bb00010
    .word  0x0c108f70
    .word  0x8e04000c
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end MUTEX_unlock
