.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBucketMan_add
.ent cBucketMan_add
cBucketMan_add:
    .word  0x24020044
    .word  0x8c830004
    .word  0x00c23018
    .word  0x00c31821
    .word  0x8c620000
    .word  0x30420001
    .word  0x5040000f
    .word  0x8c630008
    .word  0x8c630028
    .word  0xaca30004
    .word  0x8c820004
    .word  0x00c21021
    .word  0x8c430028
    .word  0xac650008
    .word  0x8c820004
    .word  0x00461021
    .word  0x24420024
    .word  0xaca20008
    .word  0x8c830004
    .word  0x00c31821
    .word  0x03e00008
    .word  0xac650028
    .word  0xaca30004
    .word  0x8c820004
    .word  0x00c21021
    .word  0x8c430008
    .word  0xac650008
    .word  0x8c820004
    .word  0x00461021
    .word  0x24420004
    .word  0xaca20008
    .word  0x8c830004
    .word  0x00c31821
    .word  0x03e00008
    .word  0xac650008
    .word  0x00000000
.end cBucketMan_add
