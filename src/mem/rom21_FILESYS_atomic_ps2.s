.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global FILESYS_atomic
.ent FILESYS_atomic
FILESYS_atomic:
    .word  0x27bdffc0
    .word  0x0080102d
    .word  0x7fb00030
    .word  0x00c0202d
    .word  0x7fb10020
    .word  0x3c100052
    .word  0xffbf0000
    .word  0x26109ab0
    .word  0x7fb20010
    .word  0x00e0282d
    .word  0x8e120008
    .word  0x0040f809
    .word  0xae060008
    .word  0xae120008
    .word  0x0040882d
    .word  0x0c0f77ea
    .word  0x0000202d
    .word  0x0220102d
    .word  0x7bb00030
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0040
.end FILESYS_atomic
