.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplay_addCache
.ent cReplay_addCache
cReplay_addCache:
    .word  0x27bdffe0
    .word  0x0000182d
    .word  0x7fb00010
    .word  0x3c050048
    .word  0xffbf0000
    .word  0x2490048c
    .word  0x8e020000
    .word  0x1440000a
    .word  0x24630001
    .word  0x24a517b0
    .word  0x24040010
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0c09b428
    .word  0x0040202d
    .word  0x10000005
    .word  0xae020000
    .word  0x28620002
    .word  0x1440fff2
    .word  0x26100004
    .word  0x0000102d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cReplay_addCache
