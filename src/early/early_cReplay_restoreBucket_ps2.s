.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

.global cReplay_restoreBucket
.ent cReplay_restoreBucket
cReplay_restoreBucket:
    .word  0x27bdffa0
    .word  0x7fb00050
    .word  0x7fb10040
    .word  0x0000802d
    .word  0x7fb20030
    .word  0x00c0882d
    .word  0x7fb30020
    .word  0x00a0902d
    .word  0xffbf0010
    .word  0x0080982d
    .word  0x03a0282d
    .word  0x24060008
    .word  0x8e220000
    .word  0x84440010
    .word  0x8c430014
    .word  0x0060f809
    .word  0x02242021
    .word  0x8fa20000
    .word  0x18400009
    .word  0x0260202d
    .word  0x0240282d
    .word  0x0220302d
    .word  0x0c09b7b6
    .word  0x26100001
    .word  0x8fa20000
    .word  0x0202102a
    .word  0x1440fff9
    .word  0x0260202d
    .word  0x27842898
    .word  0x0c0d5326
    .word  0x0240282d
    .word  0x7bb00050
    .word  0x7bb10040
    .word  0x7bb20030
    .word  0x7bb30020
    .word  0xdfbf0010
    .word  0x03e00008
    .word  0x27bd0060
.end cReplay_restoreBucket
