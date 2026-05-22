.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cUITemplate_MAP_setupEventUnlock
.ent cUITemplate_MAP_setupEventUnlock
cUITemplate_MAP_setupEventUnlock:
    .word  0x8f82f4a4
    .word  0x24030001
    .word  0xac80001c
    .word  0x2c420002
    .word  0xac830010
    .word  0x10400003
    .word  0xac830014
    .word  0x03e00008
    .word  0xac800018
    .word  0x03e00008
    .word  0xac830018
.end cUITemplate_MAP_setupEventUnlock
