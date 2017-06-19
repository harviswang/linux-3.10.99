//*****************************************************************************
//
// tcu.h - Defines and Macros for the TCU.
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

#ifndef __TCU_H__
#define __TCU_H__

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
// Values that can be passed to TCUxxxx as the parameter ulTimerID
//
//*****************************************************************************
#define TCU_TIMER7              0x00000080  // Timer7 ID
#define TCU_TIMER6              0x00000040  // Timer6 ID
#define TCU_TIMER5              0x00000020  // Timer5 ID
#define TCU_TIMER4              0x00000010  // Timer4 ID
#define TCU_TIMER3              0x00000008  // Timer3 ID
#define TCU_TIMER2              0x00000004  // Timer2 ID
#define TCU_TIMER1              0x00000002  // Timer1 ID
#define TCU_TIMER0              0x00000001  // Timer0 ID

//*****************************************************************************
//
// Values that can be passed to TCUPWMShutdown as the parameter ulShutdown
//
//*****************************************************************************
#define TCU_SHUTDOWN_GRACEFUL   0x80000000  // Graceful shutdown PWM output
#define TCU_SHUTDOWN_ABRUPT     0x40000000  // Abrupt shutdown PWM output

//*****************************************************************************
//
// Values that can be passed to TCUClockInputPrescaleSet as the
// ulClockInputPrescale parameter
//
//*****************************************************************************
#define TCU_CLOCKPRESCALE_1    0x08000000  // Internal clock: ClockInput/1
#define TCU_CLOCKPRESCALE_4    0x04000000  // Internal clock: ClockInput/4
#define TCU_CLOCKPRESCALE_16   0x02000000  // Internal clock: ClockInput/16
#define TCU_CLOCKPRESCALE_64   0x01000000  // Internal clock: ClockInput/64
#define TCU_CLOCKPRESCALE_256  0x00800000  // Internal clock: ClockInput/256
#define TCU_CLOCKPRESCALE_1024 0x00400000  // Internal clock: ClockInput/1024

//*****************************************************************************
//
// Values that can be passed to TCUClockInputSet as the
// ulClockInput parameter
//
//*****************************************************************************
#define TCU_CLOCKINPUT_EXTAL    0x00200000  // EXTAL clock as OST clock input
#define TCU_CLOCKINPUT_RTC      0x00100000  // RTC clock as OST clock input
#define TCU_CLOCKINPUT_PCLK     0x00080000  // PCLK clock as OST clock input

//*****************************************************************************
//
// Values that can be passed to TCUComparisonMatchFlagSet/Clear as the
// ulFlagType parameter
//
//*****************************************************************************
#define TCU_FLAGTYPE_FIFOEMPTY  0x00040000  // FIFO empty flag
#define TCU_FLAGTYPE_HALF       0x00020000  // HALF comparison match flag
#define TCU_FLAGTYPE_FIFOFULL   0x00010000  // FIFO comparison match flag
#define TCU_FLAGTYPE_FULL       0x00008000  // FULL comparison match flag

//*****************************************************************************
//
// Values that can be passed to TCUPWMInitialOutputLevelSet as the
// ulPWMInitialOutputLevel parameter
//
//*****************************************************************************
#define TCU_PWMINITOUTPUT_LOW   0x00004000  // PWM initial output low
#define TCU_PWMINITOUTPUT_HIGH  0x00002000  // PWM initial output high

//*****************************************************************************
//
// Values that can be passed to TCUInterruptMask as the
// ulMaskType parameter
//
//*****************************************************************************
#define TCU_MASKTYPE_FIFOEMPTY  0x00001000  // TCU FIFOEMPTY interrupt mask
#define TCU_MASKTYPE_HALF       0x00000800  // TCU HALF interrupt mask
#define TCU_MASKTYPE_FIFO       0x00000400  // TCU FIFO interrupt mask
#define TCU_MASKTYPE_FULL       0x00000200  // TCU FULL interrupt mask

//*****************************************************************************
//
// Values that can be passed to TCUMOdeSet as the ulMode parmeter
//
//*****************************************************************************
#define TCU_MODE_NONFIFO        0x00000100  // Non FIFO mode
#define TCU_MODE_FIFO           0x00000080  // FIFO mode

//*****************************************************************************
//
// Values that can be passed to TCUFIFOMOdeSet as the ulFIFOMode parmeter
//
//*****************************************************************************
#define TCU_FIFOMODE_FIFO1      0x00000040  // FIFO FIFO1 mode
#define TCU_FIFOMODE_FIFO2      0x00000020  // FIFO FIFO2 mode

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern long TCUIntNumberGet(unsigned long ulTimerID);
extern tBoolean TCUCounterEnable(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUCounterDisable(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUClockSupply(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUInterruptMask(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMaskType);
extern tBoolean TCUInterruptUnmask(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMaskType);
extern void TCUDataFullSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulDataFull);
extern void TCUDataHalfSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulDataHalf);
extern tBoolean TCUClockNotSupply(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUPWMShutdown(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulShutdown);
extern void TCUCounterSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulCounter);
extern tBoolean TCUPWMEnable(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUPWMDisable(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUPWMInitialOutputLevelSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulPWMInitialOutputLevel);
extern tBoolean TCUClockInputPrescaleSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulCounterClockPrescale);
extern tBoolean TCUClockInputSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulClockInput);
extern tBoolean TCUComparisonMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFlagType);
extern tBoolean TCUComparisonMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFlagType);
extern void TCURegisterDump(unsigned long ulBase, int (*print)(const char *format, ...));
extern tBoolean TCUModeSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMode);
extern tBoolean TCUFIFOModeSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFIFOMode);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __TCU_H__ */

