.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cActiveTriggerList_purge
.ent cActiveTriggerList_purge
cActiveTriggerList_purge:
    .word  0x27bdffd0
    .word  0x7fb10010
    .word  0xffbf0000
    .word  0x0080882d
    .word  0x7fb00020
    .word  0x8e300000
    .word  0x52000008
    .word  0xae200000
    .word  0x0200202d
    .word  0x00000000
    .word  0x0c0c5f94
    .word  0x8e100004
    .word  0x1600fffd
    .word  0x0200202d
    .word  0xae200000
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
.end cActiveTriggerList_purge
