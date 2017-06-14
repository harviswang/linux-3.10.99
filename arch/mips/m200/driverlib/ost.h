//*****************************************************************************
//
// ost.h - Defines and Macros for the OST.
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

#ifndef __OST_H__
#define __OST_H__

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
// Values that can be passed to OSTShutdown as the ulShutdownMode parameter
//
//*****************************************************************************
#define OST_SHUTDOWN_GRACEFUL   0x80000000  // Graceful shutdown counter
#define OST_SHUTDOWN_ABRUPT     0x40000000  // Abrupt shutdown counter

//*****************************************************************************
//
// Values that can be passed to OSTSetCounterMode as the ulCounterMode parameter
//
//*****************************************************************************
#define OST_COUNTER_BECLEARED   0x20000000  // When the value counter is equal to 
                                            // compare value, the counter will
                                            // (0) be cleared and increase from 0
#define OST_COUNTER_GOON        0x10000000  // (1) go on increasing till overflow,
                                            //     and then increase from 0

//*****************************************************************************
//
// Values that can be passed to OSTSetClockInputPrescale as the 
// ulClockInputPrescale parameter
//
//*****************************************************************************
#define OST_CLOCKPRESCALE_1    0x08000000  // Internal clock: ClockInput/1 
#define OST_CLOCKPRESCALE_4    0x04000000  // Internal clock: ClockInput/4 
#define OST_CLOCKPRESCALE_16   0x02000000  // Internal clock: ClockInput/16 
#define OST_CLOCKPRESCALE_64   0x01000000  // Internal clock: ClockInput/64 
#define OST_CLOCKPRESCALE_256  0x00800000  // Internal clock: ClockInput/256 
#define OST_CLOCKPRESCALE_1024 0x00400000  // Internal clock: ClockInput/1024 

//*****************************************************************************
//
// Values that can be passed to OSTSetClockInput as the ulClockInput parameter
//
//*****************************************************************************
#define OST_CLOCKINPUT_EXTAL    0x00200000  // EXTAL clock as OST clock input 
#define OST_CLOCKINPUT_RTC      0x00100000  // RTC clock as OST clock input
#define OST_CLOCKINPUT_PCLK     0x00080000  // PCLK clock as OST clock input 

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern long OSTIntNumberGet(unsigned long ulBase);
extern void OSTCounterModeSet(unsigned long ulBase, unsigned long ulCounterMode);
extern unsigned long OSTCounterModeGet(unsigned long ulBase);
extern void OSTShutdown(unsigned long ulBase, unsigned long ulShutdownMode);
extern void OSTClockInputPrescaleSet(unsigned long ulBase, unsigned long ulClockInputPrescale);
extern void OSTClockInputSet(unsigned long ulBase, unsigned long ulClockInput);
extern void OSTDataSet(unsigned long ulBase, unsigned long ulData);
extern unsigned long OSTDataGet(unsigned long ulBase);
extern void OSTCounterSet(unsigned long ulBase, unsigned long ulCounterHigh, unsigned long ulCounterLow);
extern void OSTCounterGet(unsigned long ulBase, unsigned long *pulCounterHigh, unsigned long *pulCounterLow);
extern void OSTRegisterDump(unsigned long ulBase, int (*print)(const char *format, ...));
extern tBoolean OSTCounterEnable(unsigned long ulBase);
extern tBoolean OSTCounterDisable(unsigned long ulBase);
extern tBoolean OSTInterruptUnmask(unsigned long ulBase);
extern tBoolean OSTInterruptMask(unsigned long ulBase);
extern tBoolean OSTClockSupply(unsigned long ulBase);
extern tBoolean OSTClockNotSupply(unsigned long ulBase);
extern tBoolean OSTComparisonMatchFlagSet(unsigned long ulBase);
extern tBoolean OSTComparisonMatchFlagClear(unsigned long ulBase);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __OST_H__ */

