.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEMemCard_createReadBuffer
.ent cFEMemCard_createReadBuffer
cFEMemCard_createReadBuffer:
    .word  0x27bdffd0
    .word  0x24020001
    .word  0x7fb00020
    .word  0x7fb10010
    .word  0x0080802d
    .word  0xffbf0000
    .word  0x8e030228
    .word  0x14620008
    .word  0x00a0882d
    .word  0x8e0401d4
    .word  0x50800004
    .word  0xae0001d4
    .word  0x0c0c5fa6
    .word  0x00000000
    .word  0xae0001d4
    .word  0xae000228
    .word  0x3c050046
    .word  0x0220202d
    .word  0x24a57990
    .word  0x24060100
    .word  0x0c0c5f8c
    .word  0x0000382d
    .word  0x24030001
    .word  0xae0201d4
    .word  0xae030228
    .word  0xae0201d0
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cFEMemCard_createReadBuffer
