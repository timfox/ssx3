.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cOVTemplate_PauseMenu_onWidgetEvent
.ent cOVTemplate_PauseMenu_onWidgetEvent
cOVTemplate_PauseMenu_onWidgetEvent:
    .word  0x27bdffb0
    .word  0x7fb00040
    .word  0x7fb10030
    .word  0x00a0802d
    .word  0x7fb20020
    .word  0x0080882d
    .word  0x7fb30010
    .word  0x1200011a
    .word  0xffbf0000
    .word  0x24c3ffff
    .word  0x2c620008
    .word  0x10400116
    .word  0x3c020047
    .word  0x00031880
    .word  0x2442f880
    .word  0x00621821
    .word  0x8c640000
    .word  0x00800008
    .word  0x00000000
.end cOVTemplate_PauseMenu_onWidgetEvent
