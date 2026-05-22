.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cFEAsyncManager_GetFileStatus
.ent cFEAsyncManager_GetFileStatus
cFEAsyncManager_GetFileStatus:
    .word  0x2402011c
    .word  0x00a21818
    .word  0x00642021
    .word  0x03e00008
    .word  0x8c820114
    .word  0x00000000
.end cFEAsyncManager_GetFileStatus
