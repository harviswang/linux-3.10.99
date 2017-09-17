//*****************************************************************************
//
// bitops.h - Bit operation APIs.
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
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
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

#ifndef __BITOPS_H__
#define __BITOPS_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Return the bit position (0..31) of the most significant 1 bit in a word
 * Returns -1 if no 1 bit exists
 */
static inline unsigned int fls_32bit_or_64bit(unsigned long word)
{
    int num;

    if (sizeof(word) * 8 == 32) {
        __asm__(
        "   .set    push                    \n"
        "   .set    mips32                  \n"
        "   clz %0, %1                  \n"
        "   .set    pop                 \n"
        : "=r" (num)
        : "r" (word));

        return 31 - num;
    } else if (sizeof(word) * 8 == 64) {
        __asm__(
        "   .set    push                    \n"
        "   .set    mips64                  \n"
        "   dclz    %0, %1                  \n"
        "   .set    pop                 \n"
        : "=r" (num)
        : "r" (word));

        return 63 - num;
    } else {
        return -1; // TODO
    }
}

/*
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Returns 0..SZLONG-1
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned int ffs_32bit_or_64bit(unsigned long word)
{
    return fls_32bit_or_64bit(word & -word);
}

//*****************************************************************************
//
// From the least significant bit to the most significant bit 
// LAST_BIT_INDEX(0) will get -1
//
//*****************************************************************************
#define FIRST_BIT_INDEX(x) (ffs_32bit_or_64bit(x))
#define LAST_BIT_INDEX(x)  (fls_32bit_or_64bit(x))

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __BITOPS_H__ */
