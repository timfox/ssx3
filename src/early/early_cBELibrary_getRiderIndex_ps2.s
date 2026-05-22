.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBELibrary_getRiderIndex
.ent cBELibrary_getRiderIndex
cBELibrary_getRiderIndex:
    .word  0x3c030053
    .word  0x3c020053
    .word  0x24485b20
    .word  0x246605b0
    .word  0x0000282d
    .word  0x2407001c
    .word  0x8cc20000
    .word  0x00471818
    .word  0x00681021
    .word  0x8c430010
    .word  0x30630001
    .word  0x54640003
    .word  0x24a50001
    .word  0x03e00008
    .word  0x00a0102d
    .word  0x28a20006
    .word  0x1440fff5
    .word  0x24c60004
    .word  0x03e00008
    .word  0x2402ffff
.end cBELibrary_getRiderIndex
