.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAirPredictor_reset
.ent cAirPredictor_reset
cAirPredictor_reset:
    .word  0xac8000ac
    .word  0x3c030050
    .word  0xac8000a0
    .word  0x2463f160
    .word  0xac800098
    .word  0x3c060050
    .word  0xac80009c
    .word  0x24c6f120
    .word  0xac8000a4
    .word  0x3c05ffff
    .word  0x34a5ffff
    .word  0x2407ffff
    .word  0x78620000
    .word  0x3c08004a
    .word  0x7c820020
    .word  0x78c30000
    .word  0xac870090
    .word  0x7c830010
    .word  0xac850030
    .word  0x25065960
    .word  0x68c20007
    .word  0x6cc20000
    .word  0xb082003b
    .word  0xb4820034
    .word  0x03e00008
    .word  0x00000000
.end cAirPredictor_reset
