.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global queueadd
.ent queueadd
queueadd:
    .word  0x27bdffc0
    .word  0x7fb00030
    .word  0x7fb10020
    .word  0x3c100052
    .word  0x7fb20010
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x00a0902d
    .word  0x0c0f95c0
    .word  0x26049c08
    .word  0x8e220000
    .word  0x54400003
    .word  0x8e220004
    .word  0x10000002
    .word  0xae320000
    .word  0xac520004
    .word  0xae320004
    .word  0x26049c08
    .word  0x0c0f95d8
    .word  0xae400004
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
.end queueadd
