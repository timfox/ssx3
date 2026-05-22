.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global SYSTEM_abortmessage
.ent SYSTEM_abortmessage
SYSTEM_abortmessage:
    .word  0x27bdfd70
    .word  0xffbf0200
    .word  0xffa50258
    .word  0xffa60260
    .word  0xffa70268
    .word  0xffa80270
    .word  0xffa90278
    .word  0xffaa0280
    .word  0xffab0288
    .word  0xe7ac0248
    .word  0xe7ae024c
    .word  0xe7b00250
    .word  0x10800007
    .word  0xe7b20254
    .word  0x0080282d
    .word  0x27a60258
    .word  0x0c1061b2
    .word  0x03a0202d
    .word  0x10000003
    .word  0x3c050049
    .word  0xa3a00000
    .word  0x3c050049
    .word  0x03a0302d
    .word  0x24040002
    .word  0x0c0f974c
    .word  0x24a55ef0
    .word  0x3c020045
    .word  0x8c46100c
    .word  0x10c00006
    .word  0x3c020045
    .word  0x3c050049
    .word  0x8c471010
    .word  0x24a55f00
    .word  0x0c0f974c
    .word  0x24040002
    .word  0x0c0f9284
    .word  0x00000000
    .word  0xdfbf0200
    .word  0x03e00008
    .word  0x27bd0290
.end SYSTEM_abortmessage
