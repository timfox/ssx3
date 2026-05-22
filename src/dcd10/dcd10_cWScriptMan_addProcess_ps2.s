.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cWScriptMan_addProcess
.ent cWScriptMan_addProcess
cWScriptMan_addProcess:
    .word  0x27bdffb0
    .word  0x7fb00040
    .word  0x00a0802d
    .word  0x7fb10030
    .word  0x7fb20020
    .word  0x00c0882d
    .word  0x7fb30010
    .word  0x00e0902d
    .word  0x0080982d
    .word  0x3c050049
    .word  0xffbf0000
    .word  0x24040060
    .word  0x24a59908
    .word  0x3c062000
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0200302d
    .word  0x0040202d
    .word  0x0220382d
    .word  0x0c0c1d66
    .word  0x0260282d
    .word  0x12400006
    .word  0x0040802d
    .word  0x266402bc
    .word  0x0c0c2c98
    .word  0x0200282d
    .word  0x10000005
    .word  0x0200102d
    .word  0x266402b4
    .word  0x0c0c2c98
    .word  0x0200282d
    .word  0x0200102d
    .word  0x7bb10030
    .word  0x7bb00040
    .word  0x7bb20020
    .word  0x7bb30010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0050
    .word  0x00000000
.end cWScriptMan_addProcess
