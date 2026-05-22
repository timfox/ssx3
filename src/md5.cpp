#include "md5.h"
#include "common.h"

#ifdef SSX3_HOST
#include <cstdint>
#include <cstring>
#else
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
static void md5_memcpy(void* dst, const void* src, int nbytes) {
    char* d = static_cast<char*>(dst);
    const char* s = static_cast<const char*>(src);
    int i;
    for (i = 0; i < nbytes; ++i) {
        d[i] = s[i];
    }
}
#endif

#ifdef SSX3_HOST
extern "C" {
#endif

namespace {
#ifdef SKIP_ASM

struct Md5State {
    uint32_t state[4];
    uint32_t count[2];
    uint8_t buffer[64];
};

static const uint8_t kMd5Padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static uint32_t rol(uint32_t value, unsigned int bits) {
    return (value << bits) | (value >> (32 - bits));
}

void md5_transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];

    uint32_t x[16];
    int i;
    for (i = 0; i < 16; ++i) {
        x[i] = ((uint32_t)block[i * 4]) |
               (((uint32_t)block[i * 4 + 1]) << 8) |
               (((uint32_t)block[i * 4 + 2]) << 16) |
               (((uint32_t)block[i * 4 + 3]) << 24);
    }

#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define STEP(f, a, b, c, d, x, s, ac) \
    (a) += f((b), (c), (d)) + (x) + (ac); \
    (a) = rol((a), (s)); \
    (a) += (b);

    STEP(F, a, b, c, d, x[0], 7, 0xd76aa478);
    STEP(F, d, a, b, c, x[1], 12, 0xe8c7b756);
    STEP(F, c, d, a, b, x[2], 17, 0x242070db);
    STEP(F, b, c, d, a, x[3], 22, 0xc1bdceee);
    STEP(F, a, b, c, d, x[4], 7, 0xf57c0faf);
    STEP(F, d, a, b, c, x[5], 12, 0x4787c62a);
    STEP(F, c, d, a, b, x[6], 17, 0xa8304613);
    STEP(F, b, c, d, a, x[7], 22, 0xfd469501);
    STEP(F, a, b, c, d, x[8], 7, 0x698098d8);
    STEP(F, d, a, b, c, x[9], 12, 0x8b44f7af);
    STEP(F, c, d, a, b, x[10], 17, 0xffff5bb1);
    STEP(F, b, c, d, a, x[11], 22, 0x895cd7be);
    STEP(F, a, b, c, d, x[12], 7, 0x6b901122);
    STEP(F, d, a, b, c, x[13], 12, 0xfd987193);
    STEP(F, c, d, a, b, x[14], 17, 0xa679438e);
    STEP(F, b, c, d, a, x[15], 22, 0x49b40821);

    STEP(G, a, b, c, d, x[1], 5, 0xf61e2562);
    STEP(G, d, a, b, c, x[6], 9, 0xc040b340);
    STEP(G, c, d, a, b, x[11], 14, 0x265e5a51);
    STEP(G, b, c, d, a, x[0], 20, 0xe9b6c7aa);
    STEP(G, a, b, c, d, x[5], 5, 0xd62f105d);
    STEP(G, d, a, b, c, x[10], 9, 0x02441453);
    STEP(G, c, d, a, b, x[15], 14, 0xd8a1e681);
    STEP(G, b, c, d, a, x[4], 20, 0xe7d3fbc8);
    STEP(G, a, b, c, d, x[9], 5, 0x21e1cde6);
    STEP(G, d, a, b, c, x[14], 9, 0xc33707d6);
    STEP(G, c, d, a, b, x[3], 14, 0xf4d50d87);
    STEP(G, b, c, d, a, x[8], 20, 0x455a14ed);
    STEP(G, a, b, c, d, x[13], 5, 0xa9e3e905);
    STEP(G, d, a, b, c, x[2], 9, 0xfcefa3f8);
    STEP(G, c, d, a, b, x[7], 14, 0x676f02d9);
    STEP(G, b, c, d, a, x[12], 20, 0x8d2a4c8a);

    STEP(H, a, b, c, d, x[5], 4, 0xfffa3942);
    STEP(H, d, a, b, c, x[8], 11, 0x8771f681);
    STEP(H, c, d, a, b, x[11], 16, 0x6d9d6122);
    STEP(H, b, c, d, a, x[14], 23, 0xfde5380c);
    STEP(H, a, b, c, d, x[1], 4, 0xa4beea44);
    STEP(H, d, a, b, c, x[4], 11, 0x4bdecfa9);
    STEP(H, c, d, a, b, x[7], 16, 0xf6bb4b60);
    STEP(H, b, c, d, a, x[10], 23, 0xbebfbc70);
    STEP(H, a, b, c, d, x[13], 4, 0x289b7ec6);
    STEP(H, d, a, b, c, x[0], 11, 0xeaa127fa);
    STEP(H, c, d, a, b, x[3], 16, 0xd4ef3085);
    STEP(H, b, c, d, a, x[6], 23, 0x04881d05);
    STEP(H, a, b, c, d, x[9], 4, 0xd9d4d039);
    STEP(H, d, a, b, c, x[12], 11, 0xe6db99e5);
    STEP(H, c, d, a, b, x[15], 16, 0x1fa27cf8);
    STEP(H, b, c, d, a, x[2], 23, 0xc4ac5665);

    STEP(I, a, b, c, d, x[0], 6, 0xf4292244);
    STEP(I, d, a, b, c, x[7], 10, 0x432aff97);
    STEP(I, c, d, a, b, x[14], 15, 0xab9423a7);
    STEP(I, b, c, d, a, x[5], 21, 0xfc93a039);
    STEP(I, a, b, c, d, x[12], 6, 0x655b59c3);
    STEP(I, d, a, b, c, x[3], 10, 0x8f0ccc92);
    STEP(I, c, d, a, b, x[10], 15, 0xffeff47d);
    STEP(I, b, c, d, a, x[1], 21, 0x85845dd1);
    STEP(I, a, b, c, d, x[8], 6, 0x6fa87e4f);
    STEP(I, d, a, b, c, x[15], 10, 0xfe2ce6e0);
    STEP(I, c, d, a, b, x[6], 15, 0xa3014314);
    STEP(I, b, c, d, a, x[13], 21, 0x4e0811a1);
    STEP(I, a, b, c, d, x[4], 6, 0xf7537e82);
    STEP(I, d, a, b, c, x[11], 10, 0xbd3af235);
    STEP(I, c, d, a, b, x[2], 15, 0x2ad7d2bb);
    STEP(I, b, c, d, a, x[9], 21, 0xeb86d391);

#undef STEP
#undef I
#undef H
#undef G
#undef F

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void md5_encode(uint8_t* out, const uint32_t* in, unsigned int len) {
    unsigned int i;
    unsigned int j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        out[j] = (uint8_t)(in[i] & 0xff);
        out[j + 1] = (uint8_t)((in[i] >> 8) & 0xff);
        out[j + 2] = (uint8_t)((in[i] >> 16) & 0xff);
        out[j + 3] = (uint8_t)((in[i] >> 24) & 0xff);
    }
}

Md5State* as_state(int* ctx) {
    return reinterpret_cast<Md5State*>(ctx);
}

#endif /* SKIP_ASM */
} // namespace

INCLUDE_ASM("md5", md5_process);
#ifdef SKIP_ASM
void md5_process(int* ctx, const unsigned char* data) {
    Md5State* state = as_state(ctx);
    if (!state) {
        return;
    }
    md5_transform(state->state, data);
}
#endif

INCLUDE_ASM("md5", md5_init);
#ifdef SKIP_ASM
void md5_init(int* ctx) {
    Md5State* state = as_state(ctx);
    if (!state) {
        return;
    }
    state->count[0] = 0;
    state->count[1] = 0;
    state->state[0] = 0x67452301u;
    state->state[1] = 0xefcdab89u;
    state->state[2] = 0x98badcfeu;
    state->state[3] = 0x10325476u;
}
#endif

INCLUDE_ASM("md5", md5_append);
#ifdef SKIP_ASM
void md5_append(int* ctx, const unsigned char* data, int nbytes) {
    Md5State* state = as_state(ctx);
    int part_len;
    int i;
    uint32_t index;
    if (!state || !data || nbytes <= 0) {
        return;
    }

    index = (state->count[0] >> 3) & 0x3f;
    state->count[0] += ((uint32_t)nbytes) << 3;
    if (state->count[0] < ((uint32_t)nbytes) << 3) {
        ++state->count[1];
    }
    state->count[1] += ((uint32_t)nbytes) >> 29;

    part_len = 64 - (int)index;
    i = 0;
    if (nbytes >= part_len) {
#ifdef SSX3_HOST
        std::memcpy(&state->buffer[index], data, part_len);
#else
        md5_memcpy(&state->buffer[index], data, part_len);
#endif
        md5_transform(state->state, state->buffer);
        for (i = part_len; i + 63 < nbytes; i += 64) {
            md5_transform(state->state, &data[i]);
        }
        index = 0;
    }
#ifdef SSX3_HOST
    std::memcpy(&state->buffer[index], &data[i], nbytes - i);
#else
    md5_memcpy(&state->buffer[index], &data[i], nbytes - i);
#endif
}
#endif

INCLUDE_ASM("md5", md5_finish);
#ifdef SKIP_ASM
void md5_finish(int* ctx, unsigned char* digest) {
    Md5State* state = as_state(ctx);
    uint8_t bits[8];
    uint32_t index;
    int pad_len;
    if (!state || !digest) {
        return;
    }

    md5_encode(bits, state->count, 8);

    index = (state->count[0] >> 3) & 0x3f;
    pad_len = (index < 56) ? (56 - (int)index) : (120 - (int)index);
    md5_append(ctx, kMd5Padding, pad_len);
    md5_append(ctx, bits, 8);

    md5_encode(digest, state->state, 16);
}
#endif

#ifdef SSX3_HOST
} // extern "C"
#endif
