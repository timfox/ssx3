.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cBELibrary_getCharacterID
.ent cBELibrary_getCharacterID
cBELibrary_getCharacterID:
    .word  0x3c020053
    .word  0x00042080
    .word  0x244205b0
    .word  0x2406001c
    .word  0x00822021
    .word  0x3c050053
    .word  0x8c830000
    .word  0x24a55b20
    .word  0x00661818
    .word  0x00651821
    .word  0x03e00008
    .word  0x80620011
.end cBELibrary_getCharacterID
