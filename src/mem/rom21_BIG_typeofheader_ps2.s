.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global BIG_typeofheader
.ent BIG_typeofheader
BIG_typeofheader:
    .word  0x0080282d
    .word  0x3403c0fb
    .word  0x90a60000
    .word  0x90a70001
    .word  0x00061200
    .word  0x00471025
    .word  0x54430003
    .word  0x90a40002
    .word  0x10000015
    .word  0x24050001
    .word  0x00071400
    .word  0x00061e00
    .word  0x90a50003
    .word  0x00621825
    .word  0x00042200
    .word  0x00641825
    .word  0x3c024249
    .word  0x00652025
    .word  0x34424746
    .word  0x14820003
    .word  0x3c034249
    .word  0x10000008
    .word  0x24050002
    .word  0x3c02ffff
    .word  0x3442ff00
    .word  0x34634700
    .word  0x00821024
    .word  0x24050003
    .word  0x00431026
    .word  0x0002280b
    .word  0x03e00008
    .word  0x00a0102d
.end BIG_typeofheader
