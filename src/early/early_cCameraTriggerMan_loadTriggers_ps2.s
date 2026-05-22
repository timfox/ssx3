.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerMan_loadTriggers
.ent cCameraTriggerMan_loadTriggers
cCameraTriggerMan_loadTriggers:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c05b05c
    .word  0x0080802d
    .word  0x54400011
    .word  0x7bb00010
    .word  0x8e020004
    .word  0x1840000c
    .word  0x0000282d
    .word  0x8e040000
    .word  0x00000000
    .word  0x00051080
    .word  0x24a50001
    .word  0x00441021
    .word  0x8c430000
    .word  0xac600020
    .word  0x8e020004
    .word  0x00a2102a
    .word  0x5440fff8
    .word  0x8e040000
    .word  0x0000102d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cCameraTriggerMan_loadTriggers
