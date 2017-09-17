//*****************************************************************************
//
// cpm.h - Defines and Macros for the CPM.
//
// Copyright (c) 2017 Harvis Wang.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#ifndef __CPM_H__
#define __CPM_H__

#include "../inc/hw_types.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
// Values that can be passed to  as the  parameter
//
//*****************************************************************************
#define Index_Prefetch_I    0x1c
#define CACHE_PREFETCH(label, size)                  \
do{                                 \
    unsigned long addr,end;                     \
    /* Prefetch codes from label */                 \
    addr = (unsigned long)(&&label) & ~(32 - 1);            \
    end = (unsigned long)(&&label + size) & ~(32 - 1);      \
    end += 32;                          \
    for (; addr < end; addr += 32) {                \
        __asm__ volatile (                  \
                ".set mips32\n\t"           \
                " cache %0, 0(%1)\n\t"          \
                ".set mips32\n\t"           \
                :                   \
                : "I" (Index_Prefetch_I), "r"(addr));   \
    }                               \
}                                   \
while(0)

//*****************************************************************************
//
// Values that can be passed to  as the  parameter
//
//*****************************************************************************
#define __SYNC()                \
    __asm__ __volatile__(           \
        ".set   push\n\t"       \
        ".set   noreorder\n\t"      \
        ".set   mips2\n\t"      \
        "sync\n\t"          \
        ".set   pop"            \
        : /* no output */       \
        : /* no input */        \
        : "memory")

#define __CKSEG1                  0xA0000000  // KSEG1 memory address
#define __FAST_IOB()                \
    __asm__ __volatile__(           \
        ".set   push\n\t"       \
        ".set   noreorder\n\t"      \
        "lw $0,%0\n\t"      \
        "nop\n\t"           \
        ".set   pop"            \
        : /* no output */       \
        : "m" (*(int *)__CKSEG1)      \
        : "memory")

#define FAST_IOB()          \
    do {                    \
        __SYNC();           \
        __FAST_IOB();       \
    } while (0)

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern tBoolean CPMApllIsBypassed(unsigned long ulBase);
extern unsigned long CPMApllGetM(unsigned long ulBase);
extern tBoolean CPMApllSetM(unsigned long ulBase, unsigned long ulM);
extern unsigned long CPMApllGetN(unsigned long ulBase);
extern tBoolean CPMApllSetN(unsigned long ulBase, unsigned long ulN);
extern unsigned long CPMApllGetNFraction(unsigned long ulBase);
extern tBoolean CPMApllSetNFraction(unsigned long ulBase, unsigned long ulNFraction);
extern tBoolean CPMApllEnable(unsigned long ulBase);
extern tBoolean CPMApllDisable(unsigned long ulBase);
extern unsigned long CPMMpllGetM(unsigned long ulBase);
extern tBoolean CPMMpllSetM(unsigned long ulBase, unsigned long ulM);
extern unsigned long CPMMpllGetN(unsigned long ulBase);
extern tBoolean CPMMpllSetN(unsigned long ulBase, unsigned long ulN);
extern unsigned long CPMMpllGetNFraction(unsigned long ulBase);
extern tBoolean CPMMpllSetNFraction(unsigned long ulBase, unsigned long ulNFraction);
extern unsigned long CPMGreatestCommonDividor(unsigned long ulA, unsigned long ulB);
extern tBoolean CPMMpllEnable(unsigned long ulBase);
extern tBoolean CPMMpllDisable(unsigned long ulBase);
extern tBoolean CPMPLLCalculateMNFraction(unsigned long ulOutputFrequency, unsigned long ulReferenceFrequency,
unsigned long *pulM, unsigned long *pulMFraction, unsigned long *pulN, unsigned long *pulNFraction);
extern tBoolean CPMClockEnable(unsigned long ulBase, unsigned long ulClockNo);
extern tBoolean CPMClockDisable(unsigned long ulBase, unsigned long ulClockNo);
extern tBoolean CPMClockIsEnabled(unsigned long ulBase, unsigned long ulClockNo);
extern unsigned long CPMClockDividerGet(unsigned long ulBase, unsigned long ulClockNo);
extern tBoolean CPMClockDividerSet(unsigned long ulBase, unsigned long ulClockNo, unsigned long ulClockDivider);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __CPM_H__ */
