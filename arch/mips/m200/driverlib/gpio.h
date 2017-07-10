//*****************************************************************************
//
// gpio.h - Defines and Macros for the GPIO module.
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

#ifndef __GPIO_H__
#define __GPIO_H__

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
// Values that can be passed to xxx as the parameter ulGPIOFunction
//
//*****************************************************************************
#define GPIO_FUNCTION_RAISEINTM 0x0000000F  // Raise edge interrupt input function, interrupt is masked
#define GPIO_FUNCTION_FALLINTM  0x0000000E  // Fall edge interrupt input function, interrupt is masked
#define GPIO_FUNCTION_HIGHINTM  0x0000000D  // High level interrupt input function, interrupt is masked
#define GPIO_FUNCTION_LOWINTM   0x0000000C  // Low level interrupt input function, interrupt is masked
#define GPIO_FUNCTION_RAISEINT  0x0000000B  // Raise edge triggered interrupt input function
#define GPIO_FUNCTION_FALLINT   0x0000000A  // Fall edge triggered interrupt input function
#define GPIO_FUNCTION_HIGHINT   0x00000009  // High level triggered interrupt input function
#define GPIO_FUNCTION_LOWINT    0x00000008  // Low level triggered interrupt input function
#define GPIO_FUNCTION_INTPUT    0x00000007  // GPIO input function
#define GPIO_FUNCTION_OUTPUT1   0x00000005  // GPIO output 1 function
#define GPIO_FUNCTION_OUTPUT0   0x00000004  // GPIO output 0 function
#define GPIO_FUNCTION_DEVICE3   0x00000003  // GPIO device 3 function
#define GPIO_FUNCTION_DEVICE2   0x00000002  // GPIO device 2 function
#define GPIO_FUNCTION_DEVICE1   0x00000001  // GPIO device 1 function
#define GPIO_FUNCTION_DEVICE0   0x00000000  // GPIO device 0 function


//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern tBoolean GPIOInterruptEnable(unsigned long ulBase, unsigned long ulPinNumber);
extern tBoolean GPIOInterruptDisable(unsigned long ulBase, unsigned long ulPinNumber);
extern tBoolean GPIOInterruptClear(unsigned long ulBase, unsigned long ulPinNumber);
extern tBoolean GPIOInterruptTypeSet(unsigned long ulBase, unsigned long ulPinNumber, unsigned long ulInterruptType);
extern tBoolean GPIOFunctionSet(unsigned long ulBase, unsigned long ulPinNumber, unsigned long ulFunction);
extern unsigned long GPIONumberBaseGet(unsigned long ulBase);
extern unsigned long GPIOBaseGet(unsigned long ulNumberBase);
extern unsigned long GPIOPinLevelGet(unsigned long ulBase, unsigned long ulPinNumber);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __OST_H__ */

