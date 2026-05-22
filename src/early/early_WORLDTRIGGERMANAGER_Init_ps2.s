.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global WORLDTRIGGERMANAGER_Init
.ent WORLDTRIGGERMANAGER_Init
WORLDTRIGGERMANAGER_Init:
    .word  0x27bdfff0
    .word  0x8f8221e4
    .word  0x1440000a
    .word  0xffbf0000
    .word  0x3c050048
    .word  0x24040344
    .word  0x24a53588
    .word  0x0000302d
    .word  0x0c0c5f5c
    .word  0x0000382d
    .word  0x0c0ad634
    .word  0x0040202d
    .word  0xaf8221e4
    .word  0xdfbf0000
    .word  0x03e00008
    .word  0x27bd0010
.end WORLDTRIGGERMANAGER_Init
