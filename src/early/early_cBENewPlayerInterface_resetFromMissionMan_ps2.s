.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBENewPlayerInterface_resetFromMissionMan
.ent cBENewPlayerInterface_resetFromMissionMan
cBENewPlayerInterface_resetFromMissionMan:
    .word  0x27bdffc0
    .word  0x7fb10020
    .word  0x00a0882d
    .word  0x7fb00030
    .word  0x7fb20010
    .word  0x0220202d
    .word  0xffbf0000
    .word  0x0c051de6
    .word  0x2412001c
    .word  0x00521018
    .word  0x3c100053
    .word  0x26104fe0
    .word  0x0220202d
    .word  0x00501021
    .word  0x0c051de6
    .word  0xa0400011
    .word  0x00521818
    .word  0x2404fffb
    .word  0x7bb10020
    .word  0x7bb20010
    .word  0xdfbf0000
    .word  0x00701021
    .word  0x8c430010
    .word  0x7bb00030
    .word  0x00641824
    .word  0xac430010
    .word  0x03e00008
    .word  0x27bd0040
.end cBENewPlayerInterface_resetFromMissionMan
