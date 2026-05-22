.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cAirPredictor_initLaunch
.ent cAirPredictor_initLaunch
cAirPredictor_initLaunch:
    .word  0xac8000ac
    .word  0xc48000a0
    .word  0x78a20000
    .word  0x7c820070
    .word  0x78c30000
    .word  0x7c830080
    .word  0x78a20000
    .word  0x7c820050
    .word  0x78c30000
    .word  0xe4800098
    .word  0x03e00008
    .word  0x7c830060
.end cAirPredictor_initLaunch
