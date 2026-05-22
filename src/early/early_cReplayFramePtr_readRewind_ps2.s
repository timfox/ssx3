.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplayFramePtr_readRewind
.ent cReplayFramePtr_readRewind
cReplayFramePtr_readRewind:
    .word  0x8c820004
    .word  0x03e00008
    .word  0xac82000c
    .word  0x00000000
.end cReplayFramePtr_readRewind
