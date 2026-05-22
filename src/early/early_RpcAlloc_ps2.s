.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global RpcAlloc
.ent RpcAlloc
RpcAlloc:
    .word  0x27bdfff0
    .word  0x3c050048
    .word  0xffbf0000
    .word  0x24a50478
    .word  0x3c062000
    .word  0x0c0c5f8c
    .word  0x0000382d
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end RpcAlloc
