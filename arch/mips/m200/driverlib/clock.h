//*****************************************************************************
//
// clock.h - Defines and Macros for the CPM clock, used in CPM and DT module.
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

#ifndef __CLOCK_H__
#define __CLOCK_H__

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
// Values that can be passed to device tree source file and CPM API parameter
//
//*****************************************************************************
// Included in clock gate 0
#define CLOCK_NO_NFI            0x00000000
#define CLOCK_NO_NEMC           0x00000001
#define CLOCK_NO_BCH            0x00000002
#define CLOCK_NO_OTG            0x00000003
#define CLOCK_NO_MSC0           0x00000004
#define CLOCK_NO_MSC1           0x00000005
#define CLOCK_NO_SSI0           0x00000006
#define CLOCK_NO_SMB0           0x00000007
#define CLOCK_NO_SMB1           0x00000008
#define CLOCK_NO_SMB2           0x00000009
#define CLOCK_NO_SMB3           0x0000000A
#define CLOCK_NO_AIC            0x0000000B
#define CLOCK_NO_MSC2           0x0000000C
#define CLOCK_NO_SADC           0x0000000D
#define CLOCK_NO_UART0          0x0000000E
#define CLOCK_NO_UART1          0x0000000F
#define CLOCK_NO_UART2          0x00000010
#define CLOCK_NO_UART3          0x00000011
#define CLOCK_NO_UART4          0x00000012
#define CLOCK_NO_SSI1           0x00000013
#define CLOCK_NO_SSI2           0x00000014
#define CLOCK_NO_PDMA           0x00000015
#define CLOCK_NO_UHC            0x00000016
#define CLOCK_NO_ISP            0x00000017
#define CLOCK_NO_LCD            0x00000018
#define CLOCK_NO_CSI            0x00000019
#define CLOCK_NO_DSI            0x0000001A
#define CLOCK_NO_PCM            0x0000001B
#define CLOCK_NO_DES            0x0000001C
#define CLOCK_NO_RTC            0x0000001D
#define CLOCK_NO_TCU            0x0000001E
#define CLOCK_NO_DDR            0x0000001F

// Included in clock gate 1
#define CLOCK_NO_VPU            0x00000020
#define CLOCK_NO_GPU            0x00000021
#define CLOCK_NO_IPU            0x00000022
#define CLOCK_NO_AHBMON         0x00000023
#define CLOCK_NO_EPD            0x00000024
#define CLOCK_NO_AES            0x00000025
#define CLOCK_NO_HASH           0x00000026
#define CLOCK_NO_DMIC           0x00000027
#define CLOCK_NO_P1             0x00000028
#define CLOCK_NO_P0             0x00000029
#define CLOCK_NO_AHB0           0x0000002A
#define CLOCK_NO_SYSOST         0x0000002B
#define CLOCK_NO_TCUEXCLK       0x0000002C
#define CLOCK_NO_DLINE          0x0000002D
#define CLOCK_NO_APB0           0x0000002E
#define CLOCK_NO_CPU            0x0000002F

// Others(not controlled in clock gate register)
#define CLOCK_NO_SCLKA          0x00000030
#define CLOCK_NO_I2S            0x00000031
#define CLOCK_NO_CIM            0x00000032
#define CLOCK_NO_APLL           0x00000033
#define CLOCK_NO_MPLL           0x00000034

// Total clock NO, is the last CLOCK_NO_XXX plus one
// If add CLOCK_NO_XXX, please change CLOCK_NO_TOTAL
#define CLOCK_NO_TOTAL          (CLOCK_NO_MPLL + 1)

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __CLOCK_H__ */
