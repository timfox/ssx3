.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_cCTActionNone
.ent get_cCTActionNone
get_cCTActionNone:
    .word  0x24030003
    .word  0x0000102d
    .word  0x03e00008
    .word  0xaca30000
.end get_cCTActionNone
