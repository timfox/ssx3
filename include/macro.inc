# Evaluate this file only once in case it's included more than once
.ifndef _MACRO_INC_GUARD
.set _MACRO_INC_GUARD, 1

.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm

.macro dlabel label
    .global \label
    \label:
.endm

.macro enddlabel label
    .size \label, . - \label
.endm

.macro endlabel label
    .size \label, . - \label
    .end \label
.endm

# Marker for functions that have not been decompiled to matching C yet.
.macro nonmatching label, size=1
    .global \label\().NON_MATCHING
    .type \label\().NON_MATCHING, @object
    .size \label\().NON_MATCHING, \size
    \label\().NON_MATCHING:
.endm

.endif
