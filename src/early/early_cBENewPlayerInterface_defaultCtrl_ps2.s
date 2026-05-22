.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewPlayerInterface_defaultCtrl
.ent cBENewPlayerInterface_defaultCtrl
cBENewPlayerInterface_defaultCtrl:
    .word  0x27bdfff0
    .word  0x3c020053
    .word  0x0080282d
    .word  0x24435b20
    .word  0xffbf0000
    .word  0x246400a8
    .word  0xa0600013
    .word  0x00000000
    .word  0x8c620010
    .word  0x34420002
    .word  0xac620010
    .word  0x2463001c
    .word  0x0064102a
    .word  0x5440fffa
    .word  0xa0600013
    .word  0x8ca2000c
    .word  0x84440010
    .word  0x8c430014
    .word  0x0060f809
    .word  0x00a42021
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
    .word  0x00000000
.end cBENewPlayerInterface_defaultCtrl
