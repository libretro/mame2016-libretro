// bionic puts the x86 register indices - REG_EAX, REG_RDI and the rest - in the
// global namespace unconditionally, where glibc keeps them behind __USE_GNU.
// MAME's x86 recompilers say "using namespace x86emit", which has its own
// REG_EAX, so on Android every use of one is ambiguous:
//
//     src/devices/cpu/drcbex86.cpp:146:2: error: reference to 'REG_EBX' is ambiguous
//
// The android build puts this directory on the include path, so this file is
// found first and renames bionic's names out of the way before handing over to
// the real header. Nothing in this tree reads mcontext_t's gregs by name.
#pragma once

#define REG_GS      BIONIC_REG_GS
#define REG_FS      BIONIC_REG_FS
#define REG_ES      BIONIC_REG_ES
#define REG_DS      BIONIC_REG_DS
#define REG_EDI     BIONIC_REG_EDI
#define REG_ESI     BIONIC_REG_ESI
#define REG_EBP     BIONIC_REG_EBP
#define REG_ESP     BIONIC_REG_ESP
#define REG_EBX     BIONIC_REG_EBX
#define REG_EDX     BIONIC_REG_EDX
#define REG_ECX     BIONIC_REG_ECX
#define REG_EAX     BIONIC_REG_EAX
#define REG_TRAPNO  BIONIC_REG_TRAPNO
#define REG_ERR     BIONIC_REG_ERR
#define REG_EIP     BIONIC_REG_EIP
#define REG_CS      BIONIC_REG_CS
#define REG_EFL     BIONIC_REG_EFL
#define REG_UESP    BIONIC_REG_UESP
#define REG_SS      BIONIC_REG_SS
#define REG_R8      BIONIC_REG_R8
#define REG_R9      BIONIC_REG_R9
#define REG_R10     BIONIC_REG_R10
#define REG_R11     BIONIC_REG_R11
#define REG_R12     BIONIC_REG_R12
#define REG_R13     BIONIC_REG_R13
#define REG_R14     BIONIC_REG_R14
#define REG_R15     BIONIC_REG_R15
#define REG_RDI     BIONIC_REG_RDI
#define REG_RSI     BIONIC_REG_RSI
#define REG_RBP     BIONIC_REG_RBP
#define REG_RBX     BIONIC_REG_RBX
#define REG_RDX     BIONIC_REG_RDX
#define REG_RAX     BIONIC_REG_RAX
#define REG_RCX     BIONIC_REG_RCX
#define REG_RSP     BIONIC_REG_RSP
#define REG_RIP     BIONIC_REG_RIP
#define REG_CSGSFS  BIONIC_REG_CSGSFS
#define REG_OLDMASK BIONIC_REG_OLDMASK
#define REG_CR2     BIONIC_REG_CR2

#include_next <sys/ucontext.h>

#undef REG_GS
#undef REG_FS
#undef REG_ES
#undef REG_DS
#undef REG_EDI
#undef REG_ESI
#undef REG_EBP
#undef REG_ESP
#undef REG_EBX
#undef REG_EDX
#undef REG_ECX
#undef REG_EAX
#undef REG_TRAPNO
#undef REG_ERR
#undef REG_EIP
#undef REG_CS
#undef REG_EFL
#undef REG_UESP
#undef REG_SS
#undef REG_R8
#undef REG_R9
#undef REG_R10
#undef REG_R11
#undef REG_R12
#undef REG_R13
#undef REG_R14
#undef REG_R15
#undef REG_RDI
#undef REG_RSI
#undef REG_RBP
#undef REG_RBX
#undef REG_RDX
#undef REG_RAX
#undef REG_RCX
#undef REG_RSP
#undef REG_RIP
#undef REG_CSGSFS
#undef REG_OLDMASK
#undef REG_CR2
