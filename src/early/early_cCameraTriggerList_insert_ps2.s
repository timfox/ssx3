.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCameraTriggerList_insert
.ent cCameraTriggerList_insert
cCameraTriggerList_insert:
    .word  0x04c3000e
    .word  0x8c830000
    .word  0x8c830004
    .word  0x28620078
    .word  0x54400003
    .word  0x8c820000
    .word  0x03e00008
    .word  0x0000102d
    .word  0x00031880
    .word  0x00621821
    .word  0xac650000
    .word  0x8c820004
    .word  0x24420001
    .word  0x10000004
    .word  0xac820004
    .word  0x00061080
    .word  0x00431021
    .word  0xac450000
    .word  0x03e00008
    .word  0x24020001
.end cCameraTriggerList_insert
