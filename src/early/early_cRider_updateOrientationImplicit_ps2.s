.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_updateOrientationImplicit
.ent cRider_updateOrientationImplicit
cRider_updateOrientationImplicit:
    .word  0x248201a0
    .word  0xd8830120
    .word  0x4b000183
    .word  0x4be3192a
    .word  0x4b04203d
    .word  0x4b0430be
    .word  0x4b04310b
    .word  0x4a6403be
    .word  0x4a0003bf
    .word  0x4be0195c
    .word  0xf8850120
    .word  0xd8840120
    .word  0xd8830110
    .word  0x4bc00043
    .word  0x4bc42168
    .word  0x4a2a52ac
    .word  0x4a2b5aec
    .word  0x4a2c632c
    .word  0x4bc429aa
    .word  0x4bc429db
    .word  0x4bc42afe
    .word  0x4bc00229
    .word  0x4b06087d
    .word  0x4b060a8e
    .word  0x4a86087e
    .word  0x4a860acc
    .word  0x4a46087c
    .word  0x4a460b0d
    .word  0x4a88003e
    .word  0x4a870a8a
    .word  0x4a48003c
    .word  0x4a470ac8
    .word  0x4a88003c
    .word  0x4a870b0c
    .word  0x4a48003d
    .word  0x4a470a8d
    .word  0x4b08003e
    .word  0x4b070ace
    .word  0x4b08003d
    .word  0x4b070b09
    .word  0xf8430030
    .word  0xf84a0000
    .word  0xf84b0010
    .word  0xf84c0020
    .word  0x03e00008
    .word  0x00000000
.end cRider_updateOrientationImplicit
