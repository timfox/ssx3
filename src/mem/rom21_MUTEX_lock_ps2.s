.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global MUTEX_lock
.ent MUTEX_lock
MUTEX_lock:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c108f24
    .word  0x0080802d
    .word  0x8e030004
    .word  0x14620005
    .word  0x00000000
    .word  0x8e020008
    .word  0x24420001
    .word  0x10000009
    .word  0xae020008
    .word  0x0c108f78
    .word  0x8e04000c
    .word  0x0c108f24
    .word  0x00000000
    .word  0x8e030008
    .word  0xae020004
    .word  0x24630001
    .word  0xae030008
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end MUTEX_lock
