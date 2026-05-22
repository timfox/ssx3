.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cRider_getMass
.ent cRider_getMass
cRider_getMass:
    .word  0x27bdffd0
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0xffbf0000
    .word  0x0c053720
    .word  0x0080802d
    .word  0x0040202d
    .word  0x0c053756
    .word  0x24050002
    .word  0x8e05086c
    .word  0x0c053bcc
    .word  0x0040202d
    .word  0x0c053720
    .word  0x0040882d
    .word  0x0040202d
    .word  0x0c053756
    .word  0x24050003
    .word  0x8e05086c
    .word  0x0040202d
    .word  0x0c0523d4
    .word  0x8e060b34
    .word  0xc781869c
    .word  0x44911800
    .word  0x468018e0
    .word  0xc60202fc
    .word  0x46010002
    .word  0x3c013f80
    .word  0x44812000
    .word  0x3c014120
    .word  0x44810800
    .word  0x7bb00020
    .word  0x46040000
    .word  0x7bb10010
    .word  0x46011082
    .word  0xdfbf0000
    .word  0x460018c2
    .word  0x46041080
    .word  0x46021802
    .word  0x03e00008
    .word  0x27bd0030
.end cRider_getMass
