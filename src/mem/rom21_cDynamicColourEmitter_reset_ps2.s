.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cDynamicColourEmitter_reset
.ent cDynamicColourEmitter_reset
cDynamicColourEmitter_reset:
    .word  0x27bdffe0
    .word  0x7fb00010
    .word  0xffbf0000
    .word  0x0c0dc358
    .word  0x0080802d
    .word  0x8e020178
    .word  0x18400017
    .word  0x0000202d
    .word  0x24050001
    .word  0x8f8228ac
    .word  0x54400007
    .word  0x8e020204
    .word  0xa38028a8
    .word  0xa38028a9
    .word  0xa38028aa
    .word  0xa38028ab
    .word  0xaf8528ac
    .word  0x8e020204
    .word  0x00041880
    .word  0x24840001
    .word  0x00621821
    .word  0x278828a8
    .word  0x89020003
    .word  0x99020000
    .word  0xa8620003
    .word  0xb8620000
    .word  0x8e020178
    .word  0x0082102a
    .word  0x1440ffed
    .word  0x8f8228ac
    .word  0x7bb00010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0020
.end cDynamicColourEmitter_reset
