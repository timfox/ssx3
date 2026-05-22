.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global UIAsciiToUnicode
.ent UIAsciiToUnicode
UIAsciiToUnicode:
    .word  0x10000007
    .word  0x80a20000
    .word  0x00031600
    .word  0x24a50001
    .word  0x00021603
    .word  0xa4820000
    .word  0x24840002
    .word  0x80a20000
    .word  0x1440fff9
    .word  0x90a30000
    .word  0x03e00008
    .word  0xa4800000
.end UIAsciiToUnicode
