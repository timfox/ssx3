.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cPSPLightMan_cPSPLightMan
.ent cPSPLightMan_cPSPLightMan
cPSPLightMan_cPSPLightMan:
    .word  0x3c030049
    .word  0x0080102d
    .word  0x246336d8
    .word  0xac830000
    .word  0x03e00008
    .word  0xaf842a9c
.end cPSPLightMan_cPSPLightMan
