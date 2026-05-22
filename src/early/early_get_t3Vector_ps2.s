.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global get_t3Vector
.ent get_t3Vector
get_t3Vector:
    .word  0x27bdff90
    .word  0x7fb00060
    .word  0x7fb10050
    .word  0x03a0802d
    .word  0x7fb20040
    .word  0x24110002
    .word  0x7fb30030
    .word  0x0000902d
    .word  0x7fb40020
    .word  0x0080982d
    .word  0xffbf0010
    .word  0x00a0a02d
    .word  0x0200282d
    .word  0x0c0b4944
    .word  0x0260202d
    .word  0x26100004
    .word  0x2631ffff
    .word  0x0621fffa
    .word  0x02429021
    .word  0x6ba30007
    .word  0x6fa30000
    .word  0x8fa40008
    .word  0xb2830007
    .word  0xb6830000
    .word  0x0240102d
    .word  0xae840008
    .word  0x7bb00060
    .word  0x7bb10050
    .word  0x7bb20040
    .word  0x7bb30030
    .word  0x7bb40020
    .word  0xdfbf0010
    .word  0x03e00008
    .word  0x27bd0070
.end get_t3Vector
