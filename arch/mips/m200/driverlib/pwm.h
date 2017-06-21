//*****************************************************************************
//
// pwm.h - Defines and Macros for the PWM module(the sub of TCU mode).
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

#ifndef __PWM_H__
#define __PWM_H__

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
// Values that can be passed to PWMEnable/PWMDisable/PWMConfigure as the parameter ulPWMIndex
//
//*****************************************************************************
#define PWM_INDEX_7             0x00000007
#define PWM_INDEX_6             0x00000006
#define PWM_INDEX_5             0x00000005
#define PWM_INDEX_4             0x00000004
#define PWM_INDEX_3             0x00000003
#define PWM_INDEX_2             0x00000002
#define PWM_INDEX_1             0x00000001
#define PWM_INDEX_0             0x00000000

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern tBoolean PWMConfigure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long ulDuty, unsigned long ulPeriod);
extern tBoolean PWMEnable(unsigned long ulBase, unsigned long ulPWMIndex);
extern tBoolean PWMDisable(unsigned long ulBase, unsigned long ulPWMIndex);
extern tBoolean PWMFIFOMode1Configure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long *pulDuty, unsigned long * pulPeriod, unsigned long ulFIFONumber);
extern tBoolean PWMFIFOMode2Configure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long *pulDuty, unsigned long * pulPeriod, unsigned long ulFIFONumber, unsigned long ulFIFOCycleNumber);
extern tBoolean TCUFIFOClear(unsigned long ulBase, unsigned long ulTimerID);
extern tBoolean TCUFIFOWrite(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulData);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __PWM_H__ */

