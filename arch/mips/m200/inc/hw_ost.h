//*****************************************************************************
//
// hw_ost.h - Macros used when accessing the OST hardware.
//
// Copyright (c) 2016-2017 Harvis Wang.  All rights reserved.
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

#ifndef __HW_OST_H__
#define __HW_OST_H__

//*****************************************************************************
//
// The following are defines for the OST register offsets.
//
//*****************************************************************************
#define OST_O_DR                0x000000E0  // OST Data Reigster
#define OST_O_CNTL              0x000000E4  // OST Counter Lower 32 Bits
#define OST_O_CNTH              0x000000E8  // OST Counter Higher 32 Bits
#define OST_O_CSR               0x000000EC  // OST Control Register(16-bit read/write)
#define OST_O_CNTHBUF           0x000000FC  // OST Counter Higher 32 Bits Buffer

//*****************************************************************************
//
// The following are defines for the bit fields in the OST_O_DR register.
//
//*****************************************************************************
#define OST_DR_DR               0xFFFFFFFF  // Store data to be compared with TCNT(it's 64bits, contains OST_O_CNTH, OST_O_CNTL)

//*****************************************************************************
//
// The following are defines for the bit fields in the OST_O_CNTL register.
//
//*****************************************************************************
#define OST_CNTL_CNT            0xFFFFFFFF  // Lower 32-bit of 64-bit TCNT

//*****************************************************************************
//
// The following are defines for the bit fields in the OST_O_CNTH register.
//
//*****************************************************************************
#define OST_CNTH_CNT            0xFFFFFFFF  // High 32-bit of 64-bit TCNT

//*****************************************************************************
//
// The following are defines for the bit fields in the OST_O_CSR register.
// X: compare value(OST_O_DR)
// CLK: OST time clock frequency
//*****************************************************************************
#define OST_CSR_CNTMD           0x00008000  // TCNT counter mode choose bit   
                                            // 0: 0 -> X -> 0
                                            // 1: 0 -> X -> Overflow -> 0
#define OST_CSR_SD              0x00000200  // OST counter mode choose bit   
                                            // 0: graceful shutdown(only when CNTMD=0)
                                            // 1: abrupt shutdown
#define OST_CSR_PRESCALE        0x00000038  // TCNT up-counter clock frequency
                                            // CLK/(4 ^ OST_CSR_PRESCALE)
#define OST_CSR_PRESCALE_1      0x00000000  // CLK/1
#define OST_CSR_PRESCALE_4      0x00000008  // CLK/4
#define OST_CSR_PRESCALE_16     0x00000010  // CLK/16
#define OST_CSR_PRESCALE_64     0x00000018  // CLK/64
#define OST_CSR_PRESCALE_256    0x00000020  // CLK/256
#define OST_CSR_PRESCALE_1024   0x00000028  // CLK/1024

#define OST_CSR_EXTEN           0x00000004  // Select EXTAL as the timer clock input 
                                            // 0: disable 
                                            // 1: enable
#define OST_CSR_RTCEN           0x00000002  // Select RTCCLK as the timer clock input 
#define OST_CSR_PCKEN           0x00000001  // Select PCLK as the timer clock input 

//*****************************************************************************
//
// The following are defines for the bit fields in the OST_O_CNTHBUF register.
//
//*****************************************************************************
#define OST_CNTHBUF_CNT         0xFFFFFFFF  // High 32-bit TCNT buffer, used when read lower 32-bit TCNT

#endif /* __HW_OST_H__ */

