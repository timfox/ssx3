.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEAsyncManager_SetFileStatus
.ent cFEAsyncManager_SetFileStatus
cFEAsyncManager_SetFileStatus:
    .word  0x2402011c
    .word  0x00a21818
    .word  0x00642021
    .word  0x03e00008
    .word  0xac860114
    .word  0x00000000
.end cFEAsyncManager_SetFileStatus
