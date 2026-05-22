.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cObjectInterface_getInstanceMan
.ent cObjectInterface_getInstanceMan
cObjectInterface_getInstanceMan:
    .word  0x8f82f7b8
    .word  0x8c430084
    .word  0x03e00008
    .word  0x8c620088
.end cObjectInterface_getInstanceMan
