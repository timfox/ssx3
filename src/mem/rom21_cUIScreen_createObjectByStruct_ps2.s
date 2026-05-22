.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUIScreen_createObjectByStruct
.ent cUIScreen_createObjectByStruct
cUIScreen_createObjectByStruct:
    .word  0x27bdff90
    .word  0x7fb40020
    .word  0x7fb50010
    .word  0x00a0a02d
    .word  0x7fb00060
    .word  0x0080a82d
    .word  0x7fb10050
    .word  0x7fb20040
    .word  0x7fb30030
    .word  0xffbf0000
    .word  0x96820000
    .word  0x2444fff0
    .word  0x2c830012
    .word  0x106001a0
    .word  0x3c020049
    .word  0x00041880
    .word  0x24423f70
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cUIScreen_createObjectByStruct
