#include "common.h"

#ifdef SSX3_HOST
#include <cstring>
#endif

#ifdef SSX3_HOST
extern "C" {
#endif
int GetHashValue32(char* param_1);
int tHashName32_getHashValue(unsigned int*, char*);
void md5_init(int* ctx);
void md5_append(int* ctx, const unsigned char* data, int nbytes);
void md5_finish(int* ctx, unsigned char* digest);
#ifdef SSX3_HOST
}
#endif


//100%
//https://decomp.me/scratch/YWMcE
INCLUDE_ASM("hashvalue", tHashName32_getHashValue__FPUiPc);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
int tHashName32_getHashValue(uint* out, char* str) {
    uint hash = 0;
    uint top;

    while (*str) {
        hash = (hash << 4) + *str++;
        top = hash & 0xF0000000;
        if (top != 0) {
            hash = (hash ^ (top >> 23)) ^ top;
        }
    }

    *out = hash;
    return *out;
}
#endif

//100%
//https://decomp.me/scratch/VD967
INCLUDE_ASM("hashvalue", GetHashValue32__FPc);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
int GetHashValue32(char* param_1) {
    char hash[4];
    return tHashName32_getHashValue((uint*)hash, param_1);
}
#endif

//99.17%
//https://decomp.me/scratch/ZrcdK
INCLUDE_ASM("hashvalue", GetHashValue64__FPc);
#ifdef SKIP_ASM
#ifdef SKIP_ASM
static int hash_str_len(const char* str) {
    int len = 0;
    if (!str) {
        return 0;
    }
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}
#endif

#ifdef SSX3_HOST
extern "C"
#endif
ulong tHashName64_getHashValue(ulong* out, char* str) {
    int ctx[24];
    unsigned char digest[16];
    int i;
    const int len =
#ifdef SSX3_HOST
        str ? static_cast<int>(std::strlen(str)) : 0;
#else
        hash_str_len(str);
#endif

    for (i = 0; i < 24; ++i) {
        ctx[i] = 0;
    }
    for (i = 0; i < 16; ++i) {
        digest[i] = 0;
    }

    md5_init(ctx);
    if (len > 0) {
        md5_append(ctx, (unsigned char*)str, len);
    }
    md5_finish(ctx, digest);

    {
        ulong low = 0;
        ulong high = 0;
        for (i = 0; i < 8; ++i) {
            low |= ((ulong)digest[i]) << (i * 8);
            high |= ((ulong)digest[i + 8]) << (i * 8);
        }
        out[0] = low;
        out[1] = high;
        return low;
    }
}
#endif


//100%
//https://decomp.me/scratch/I4NLb
INCLUDE_ASM("hashvalue", tHashName64_getHashValue__FPUlPc);
#ifdef SKIP_ASM
#ifdef SSX3_HOST
extern "C"
#endif
ulong GetHashValue64(char* str) {
    ulong hash[2];
    hash[0] = 0;
    hash[1] = 0;
    return tHashName64_getHashValue(hash, str);
}
#endif

INCLUDE_ASM("hashvalue", func_00317710);

INCLUDE_ASM("hashvalue", func_00317798);
