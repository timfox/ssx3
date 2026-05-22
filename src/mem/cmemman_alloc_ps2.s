.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

glabel cMemMan_alloc__FUlPvUlT1
    addiu   $sp, $sp, -96
    sq      $s3, 32($sp)
    move    $s3, $a2
    sq      $s4, 16($sp)
    move    $s4, $a0
    andi    $v0, $s3, 0x400
    sll     $v0, $v0, 0x15
    addiu   $v1, $s4, -16
    sra     $a2, $v0, 0x1f
    sq      $s0, 80($sp)
    sra     $v1, $v1, 0x1f
    srl     $a0, $s3, 0x18
    sq      $s1, 64($sp)
    addiu   $v1, $v1, 4
    andi    $a0, $a0, 0xf
    addiu   $a1, $a2, 1
    andi    $s0, $a2, 0x80
    sllv    $a1, $a1, $v1
    addiu   $a2, $a0, -1
    li      $v0, 4
    lui     $s1, 0x54
    sq      $s2, 48($sp)
    sllv    $v0, $v0, $a0
    addiu   $s1, $s1, -29952
    or      $s0, $s0, $a1
    sra     $a2, $a2, 0x1f
    .word   0xffbf0000                     /* sd $ra, 0($sp) */
    movz    $s0, $v0, $a2
    move    $s2, $a3
    jal     MUTEX_lock
     move   $a0, $s1
    move    $a1, $s0
    move    $a2, $s3
    move    $a3, $s2
    jal     cMemMan_allocBody
     move   $a0, $s4
    move    $s0, $v0
    jal     MUTEX_unlock
     move   $a0, $s1
    move    $v0, $s0
    lq      $s1, 64($sp)
    lq      $s0, 80($sp)
    lq      $s2, 48($sp)
    lq      $s3, 32($sp)
    lq      $s4, 16($sp)
    .word   0xdfbf0000                     /* ld $ra, 0($sp) */
    jr      $ra
     addiu  $sp, $sp, 96
