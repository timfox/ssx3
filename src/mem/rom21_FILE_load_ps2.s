.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global FILE_load
.ent FILE_load
FILE_load:
    .word  0x27bdffd0
    .word  0x3c030045
    .word  0xafa40000
    .word  0x24020001
    .word  0xafa50010
    .word  0x3c04003e
    .word  0x8c660c38
    .word  0x24841810
    .word  0xffbf0020
    .word  0x0000282d
    .word  0xafa2000c
    .word  0x0c0f777c
    .word  0x03a0382d
    .word  0xdfbf0020
    .word  0x03e00008
    .word  0x27bd0030
.end FILE_load
