.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cCommSystem_getSignature
.ent cCommSystem_getSignature
cCommSystem_getSignature:
    .word  0x3c020048
    .word  0x03e00008
    .word  0x24421370
    .word  0x00000000
.end cCommSystem_getSignature
