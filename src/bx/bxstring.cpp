#include "common.h"

INCLUDE_ASM("bx/bxstring", cBXString_cBXString1);

INCLUDE_ASM("bx/bxstring", cBXString_Realloc);

INCLUDE_ASM("bx/bxstring", cBXString_Reset);

INCLUDE_ASM("bx/bxstring", func_00318120);

INCLUDE_ASM("bx/bxstring", cBXString_Resize);

INCLUDE_ASM("bx/bxstring", func_003181E8);

INCLUDE_ASM("bx/bxstring", cBXString__cBXString);

INCLUDE_ASM("bx/bxstring", cBXString_cBXString2);

INCLUDE_ASM("bx/bxstring", func_00318350);

INCLUDE_ASM("bx/bxstring", cBXString_InitFromCString);

INCLUDE_ASM("bx/bxstring", cBXString_operatorE);

INCLUDE_ASM("bx/bxstring", cBXString_cBXString4);

INCLUDE_ASM("bx/bxstring", func_00318540);

INCLUDE_ASM("bx/bxstring", func_003185C8);

INCLUDE_ASM("bx/bxstring", func_00318630);

INCLUDE_ASM("bx/bxstring", func_003186D0);

INCLUDE_ASM("bx/bxstring", func_00318760);

INCLUDE_ASM("bx/bxstring", func_003187C8);

INCLUDE_ASM("bx/bxstring", cBXString_ConcatImpl);

//94.09%
//https://decomp.me/scratch/4rHwX
INCLUDE_ASM("bx/bxstring", cBXString_Concat);

INCLUDE_ASM("bx/bxstring", func_00318968);

INCLUDE_ASM("bx/bxstring", func_003189A0);

INCLUDE_ASM("bx/bxstring", func_003189D0);

INCLUDE_ASM("bx/bxstring", func_00318A88);

INCLUDE_ASM("bx/bxstring", func_00318AF8);

INCLUDE_ASM("bx/bxstring", func_00318B40);

INCLUDE_ASM("bx/bxstring", func_00318BC0);

/* 0x219bd8+ lives in asm/bx/bxstring_mid.s (see config/ssx3_us.yaml). */

//100%
//https://decomp.me/scratch/ETxUq
struct cBXString {
    char* pStrdata;
};

static char* bx_strchr(char* str, int ch) {
    if (!str) {
        return 0;
    }
    const unsigned char c = static_cast<unsigned char>(ch);
    for (char* p = str; *p; ++p) {
        if (static_cast<unsigned char>(*p) == c) {
            return p;
        }
    }
    return 0;
}

#ifdef SKIP_ASM
int cBXString_FindFirstOf(cBXString* str, char needle) {
    char* found = bx_strchr(str->pStrdata, static_cast<unsigned char>(needle));
    if (!found) {
        return -1;
    }
    return static_cast<int>(found - str->pStrdata);
}
#endif
