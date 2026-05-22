.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBucketMan_purgeBucket
.ent cBucketMan_purgeBucket
cBucketMan_purgeBucket:
    .word  0x27bdffb0
    .word  0x7fb20020
    .word  0x7fb30010
    .word  0x00a0902d
    .word  0x7fb00040
    .word  0x7fb10030
    .word  0xffbf0000
    .word  0x0c0d53aa
    .word  0x0080982d
    .word  0x0040802d
    .word  0x52000012
    .word  0x7bb00040
    .word  0x0260202d
    .word  0x00000000
    .word  0x0200282d
    .word  0x0c0d53be
    .word  0x0240302d
    .word  0x12000007
    .word  0x0040882d
    .word  0x8e02000c
    .word  0x24050003
    .word  0x84440008
    .word  0x8c43000c
    .word  0x0060f809
    .word  0x02042021
    .word  0x0220802d
    .word  0x5600fff3
    .word  0x0260202d
    .word  0x7bb00040
    .word  0x7bb10030
    .word  0x7bb20020
    .word  0x7bb30010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0050
    .word  0x00000000
.end cBucketMan_purgeBucket
