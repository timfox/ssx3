.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global iFILESYS_CommandCompleteCallback
.ent iFILESYS_CommandCompleteCallback
iFILESYS_CommandCompleteCallback:
    .word  0x27bdffe0
    .word  0x3c030052
    .word  0x7fb00010
    .word  0x0060302d
    .word  0xffbf0000
    .word  0x24639ab0
    .word  0x8c620010
    .word  0x8c650024
    .word  0x24420001
    .word  0xac620010
    .word  0x10a0001b
    .word  0x7bb00010
    .word  0x8ca20004
    .word  0x14400004
    .word  0x2402ffff
    .word  0x2402fffe
    .word  0x24030001
    .word  0x0064100b
    .word  0xaca20008
    .word  0x24d09ab0
    .word  0xae000024
    .word  0x8ca20028
    .word  0x10400006
    .word  0x00000000
    .word  0x8ca60014
    .word  0x8ca40000
    .word  0x8ca50008
    .word  0x0040f809
    .word  0x00000000
    .word  0x8e020010
    .word  0x2442ffff
    .word  0xae020010
    .word  0x8e030010
    .word  0x14600004
    .word  0x7bb00010
    .word  0x0c0f77ea
    .word  0x0000202d
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
    .word  0x00000000
.end iFILESYS_CommandCompleteCallback
