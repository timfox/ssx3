.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cKeyboardPopup_getObjName
.ent cKeyboardPopup_getObjName
cKeyboardPopup_getObjName:
    .word  0x27bdffc0
    .word  0x00a0402d
    .word  0x7fb00030
    .word  0xffbf0020
    .word  0x10e00009
    .word  0x00c0802d
    .word  0x3c02004a
    .word  0x24461368
    .word  0x68c30007
    .word  0x6cc30000
    .word  0xb2030007
    .word  0xb6030000
    .word  0x1000000c
    .word  0x2d020054
    .word  0x3c020046
    .word  0x2446d550
    .word  0x68c30007
    .word  0x6cc30000
    .word  0x80c40008
    .word  0x80c50009
    .word  0xb2030007
    .word  0xb6030000
    .word  0xa2040008
    .word  0xa2050009
    .word  0x2d020054
    .word  0x10400059
    .word  0x3c020046
    .word  0x00081880
    .word  0x244265b0
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cKeyboardPopup_getObjName
