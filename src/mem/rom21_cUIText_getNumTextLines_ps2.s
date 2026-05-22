.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIText_getNumTextLines
.ent cUIText_getNumTextLines
cUIText_getNumTextLines:
    .word  0x8c8200b4
    .word  0x14400003
    .word  0x0040182d
    .word  0x03e00008
    .word  0x0000102d
    .word  0x94620000
    .word  0x10400011
    .word  0x24050001
    .word  0x978616b8
    .word  0x2407000d
    .word  0x94620000
    .word  0x14460006
    .word  0x94640002
    .word  0x14820004
    .word  0x00000000
    .word  0x24630004
    .word  0x10000004
    .word  0x24a50001
    .word  0x14470002
    .word  0x24630002
    .word  0x24a50001
    .word  0x94620000
    .word  0x1440fff3
    .word  0x00000000
    .word  0x00051600
    .word  0x03e00008
    .word  0x00021603
    .word  0x00000000
.end cUIText_getNumTextLines
