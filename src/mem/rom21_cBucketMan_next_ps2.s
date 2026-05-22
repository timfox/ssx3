.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBucketMan_next
.ent cBucketMan_next
cBucketMan_next:
    .word  0x24020044
    .word  0x8c830004
    .word  0x00c23018
    .word  0x8ca20004
    .word  0x00661821
    .word  0x24630014
    .word  0x00431826
    .word  0x03e00008
    .word  0x0003100a
    .word  0x00000000
.end cBucketMan_next
