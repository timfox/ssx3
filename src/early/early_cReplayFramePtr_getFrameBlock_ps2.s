.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplayFramePtr_getFrameBlock
.ent cReplayFramePtr_getFrameBlock
cReplayFramePtr_getFrameBlock:
    .word  0x03e00008
    .word  0x00000000
.end cReplayFramePtr_getFrameBlock
