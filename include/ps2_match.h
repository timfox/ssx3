#ifndef PS2_MATCH_H
#define PS2_MATCH_H

/* Emit retail MIPS in SKIP_ASM TUs (EE gcc 2.95). Args arrive in $4/$5/$6. */
#if defined(SKIP_ASM) && !defined(SSX3_HOST)

#if defined(__cplusplus)
#define PS2_DECL0(name) extern "C" void* name(void)
#define PS2_DECL1(name) extern "C" void* name(void*)
#define PS2_DECL2(name) extern "C" void* name(void*, int)
#define PS2_DECL3(name) extern "C" void* name(void*, void*, int)
#define PS2_DECL4(name) extern "C" void* name(void*, void*, int, int)
#else
#define PS2_DECL0(name) void* name(void)
#define PS2_DECL1(name) void* name(void*)
#define PS2_DECL2(name) void* name(void*, int)
#define PS2_DECL3(name) void* name(void*, void*, int)
#define PS2_DECL4(name) void* name(void*, void*, int, int)
#endif

#define PS2_RETAIL_ASM0(name, insn_block)                                       \
    PS2_DECL0(name);                                                            \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#define PS2_RETAIL_ASM1(name, insn_block)                                       \
    PS2_DECL1(name);                                                            \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#define PS2_RETAIL_ASM2(name, insn_block)                                       \
    PS2_DECL2(name);                                                            \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#define PS2_RETAIL_ASM3(name, insn_block)                                       \
    PS2_DECL3(name);                                                            \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#define PS2_RETAIL_ASM4(name, insn_block)                                       \
    PS2_DECL4(name);                                                            \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#define PS2_RETAIL_ASM PS2_RETAIL_ASM2

/* Objdiff units: declare the function signature yourself, then embed .text only. */
#define PS2_RETAIL_ASM_ONLY(name, insn_block)                                   \
    __asm__(                                                                    \
        ".section .text, \"ax\"\n"                                              \
        ".set noat\n"                                                           \
        ".set noreorder\n"                                                      \
        ".globl " #name "\n"                                                    \
        ".ent " #name "\n"                                                      \
        #name ":\n"                                                             \
        insn_block "\n"                                                         \
        ".end " #name "\n"                                                      \
    );

#endif

#endif /* PS2_MATCH_H */
