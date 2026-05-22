.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBucketMan_addfirst
.ent cBucketMan_addfirst
cBucketMan_addfirst:
    .word  0x24020044
    .word  0x8c830004
    .word  0x00a22818
    .word  0x00a32821
    .word  0x8ca20028
    .word  0x24a50034
    .word  0x00452826
    .word  0x03e00008
    .word  0x0005100a
    .word  0x00000000
.end cBucketMan_addfirst
