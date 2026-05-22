.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cScreenPopup_onCreateScreen
.ent cScreenPopup_onCreateScreen
cScreenPopup_onCreateScreen:
    .word  0x27bdffd0
    .word  0x7fb10010
    .word  0x0080882d
    .word  0xffbf0000
    .word  0x7fb00020
    .word  0x3c04004a
    .word  0x24841c40
    .word  0x0c0c5d9c
    .word  0x8e300010
    .word  0x0200202d
    .word  0x0040302d
    .word  0x0220282d
    .word  0x0c0e5f30
    .word  0x0000382d
    .word  0x0040202d
    .word  0x10800006
    .word  0xae240040
    .word  0x24020001
    .word  0x0000282d
    .word  0xae2202b8
    .word  0x0c0e721c
    .word  0x0000302d
    .word  0x7bb00020
    .word  0x7bb10010
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0030
    .word  0x00000000
.end cScreenPopup_onCreateScreen
