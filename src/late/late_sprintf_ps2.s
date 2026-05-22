.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global sprintf
.ent sprintf
sprintf:
    .word  0x3c020045
    .word  0x27bdff20
    .word  0x8c4d4944
    .word  0x3c037fff
    .word  0x0080602d
    .word  0x3463ffff
    .word  0xffa600b0
    .word  0x24020208
    .word  0xffbf0060
    .word  0x03a0202d
    .word  0xafa30014
    .word  0x27a600b0
    .word  0xafa30008
    .word  0xffa700b8
    .word  0xffa800c0
    .word  0xffa900c8
    .word  0xffaa00d0
    .word  0xffab00d8
    .word  0xe7ac00a0
    .word  0xe7ae00a4
    .word  0xe7b000a8
    .word  0xe7b200ac
    .word  0xa7a2000c
    .word  0xafac0010
    .word  0xafad0054
    .word  0x0c106d96
    .word  0xafac0000
    .word  0x8fa30000
    .word  0xdfbf0060
    .word  0xa0600000
    .word  0x03e00008
    .word  0x27bd00e0
.end sprintf
