.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global SYNCTASK_del
.ent SYNCTASK_del
SYNCTASK_del:
    .word  0x3c020052
    .word  0x0080302d
    .word  0x8c43ed98
    .word  0x0040202d
    .word  0x10660009
    .word  0x0000282d
    .word  0x2483ed98
    .word  0x24a50001
    .word  0x28a20010
    .word  0x1040000c
    .word  0x24630010
    .word  0x8c620000
    .word  0x5446fffb
    .word  0x24a50001
    .word  0x28a20010
    .word  0x10400006
    .word  0x2482ed98
    .word  0x00051900
    .word  0x00621021
    .word  0x8c440000
    .word  0x50860001
    .word  0xac400000
    .word  0x03e00008
    .word  0x00000000
.end SYNCTASK_del
