//*****************************************************************************
//
// hw_memmap.h - Macros defining the memory of M200.
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

#ifndef __HW_MEMMAP_H__
#define __HW_MEMMAP_H__

//*****************************************************************************
//
// The following are defines for the base address of the memories and
// peripherals.
//
//*****************************************************************************
#define CPM_BASE                0xB0000000  // CPM(Clock Power Management)
#define INTC_BASE               0xB0001000  // INTC
#define OST_BASE                0xB0002000  // OST
#define TCU_BASE                0xB0002000  // TCU
#define PWM_BASE                0xB0002000  // PWM
#define GPIOA_BASE              0xB0010000  // GPIO PA
#define GPIOB_BASE              0xB0010100  // GPIO PB
#define GPIOC_BASE              0xB0010200  // GPIO PC
#define GPIOD_BASE              0xB0010300  // GPIO PD
#define GPIOE_BASE              0xB0010400  // GPIO PE
#define GPIOF_BASE              0xB0010500  // GPIO PF
#define GPIOZ_BASE              0xB0010F00  // GPIO PZ(Dummy)
#define UART0_BASE              0xB0030000  // UART0
#define UART1_BASE              0xB0031000  // UART1
#define UART2_BASE              0xB0032000  // UART2
#define UART3_BASE              0xB0033000  // UART3
#define UART4_BASE              0xB0034000  // UART4

#endif /* __HW_MEMMAP_H__ */

