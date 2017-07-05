//*****************************************************************************
//
// intc.h - Defines and Macros for the INTC.
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

#ifndef __INTC_H__
#define __INTC_H__

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
// The following are defines for the interrupt number.
//
//*****************************************************************************
#define INTC_INTERRUPT_NUM      0x00000040  // Number of interrupts

//*****************************************************************************
//
// IRQ index for INC, total 64 interrupts
//
//*****************************************************************************
enum {	IRQ_NO_DMIC,
	IRQ_NO_AIC0,
	IRQ_NO_BCH,
	IRQ_NO_DSI,
	IRQ_NO_CSI,
	IRQ_NO_OHCI,
	IRQ_NO_IPU,
	IRQ_NO_SSI1,
	IRQ_NO_SSI0,
	IRQ_NO_Reserved0,
	IRQ_NO_PDMA,
	IRQ_NO_Reserved1,
	IRQ_NO_GPIO5,
	IRQ_NO_GPIO4,
	IRQ_NO_GPIO3,
	IRQ_NO_GPIO2,
	IRQ_NO_GPIO1,
	IRQ_NO_GPIO0,
	IRQ_NO_SADC,
	IRQ_NO_EPDC,
	IRQ_NO_EHCI,
	IRQ_NO_OTG,
	IRQ_NO_HASH,
	IRQ_NO_AES,
	IRQ_NO_Reserved2,
	IRQ_NO_TCU2,
	IRQ_NO_TCU1,
	IRQ_NO_TCU0,
	IRQ_NO_Reserved3,
	IRQ_NO_ISP,
	IRQ_NO_DELAY_LINE,
	IRQ_NO_LCD,
	IRQ_NO_RTC,
	IRQ_NO_Reserved4,
	IRQ_NO_UART4,
	IRQ_NO_MSC2,
	IRQ_NO_MSC1,
	IRQ_NO_MSC0,
	IRQ_NO_Reserved5,
	IRQ_NO_NFI,
	IRQ_NO_PCM0,
	IRQ_NO_Reserved6,
	IRQ_NO_Reserved7,
	IRQ_NO_Reserved8,
	IRQ_NO_HARB2,
	IRQ_NO_HARB1,
	IRQ_NO_HARB0,
	IRQ_NO_CPM,
	IRQ_NO_UART3,
	IRQ_NO_UART2,
	IRQ_NO_UART1,
	IRQ_NO_UART0,
	IRQ_NO_DDR,
	IRQ_NO_Reserved9,
	IRQ_NO_EFUSE,
	IRQ_NO_ETHC,
	IRQ_NO_Reserved10,
	IRQ_NO_I2C3,
	IRQ_NO_I2C2,
	IRQ_NO_I2C1,
	IRQ_NO_I2C0,
	IRQ_NO_PDMAM,
	IRQ_NO_VPU,
	IRQ_NO_GPU,
};

//*****************************************************************************
//
// Hardware IRQ model used in M200 SoC
// level 0: [cpu-v0] [cpu-v1] [cpu-v2] [cpu-v3] [cpu-v4] [cpu-v5] [cpu-v6] [cpu-v7]  (cpu-level)
//                               |
// level 1: [lcd] [i2c] [gpio] [pdma] [vpu] [isp] [gpu] [efuse] [ethc] [pdmam] ...   (SoC-level)
//                        |
// level 2:  [pa0...31] [pb0...31] ... [pf0...31]                                    (SoC-level)
//*****************************************************************************
// 0 level: 8 cpu interrupts, using VINT(interrupt vector) deal with
// 1 level: 64 INTC interrupts, using INTC module deal with
// 2 level: xxx gpio interrupts, using INTC/GPIO two modules deal with
// in practice, 0 level is dealed in vector mode, so we don't care it more.
// 1/2 level is dealed with software using liner mode, [0, 63] for INTC, [64, 95]
// for GPIOPA, [96, 127] for GPIOPB andd so on

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern tBoolean INTCInterruptEnable(unsigned long ulInterrupt);
extern tBoolean INTCInterruptDisable(unsigned long ulInterrupt);
extern void INTCRegisterDump(int (*print)(const char *format, ...));
extern unsigned long INTCSr0Get(void);
extern unsigned long INTCSr1Get(void);
extern tBoolean INTCInterruptPending(unsigned long ulInterrupt);
extern unsigned long INTCDMASr0Get(void);
extern unsigned long INTCDMASr1Get(void);
extern tBoolean INTCDMAInterruptEnable(unsigned long ulInterrupt);
extern tBoolean INTCDMAInterruptDisable(unsigned long ulInterrupt);
extern tBoolean INTCDMAInterruptPending(unsigned long ulInterrupt);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __INTC_H__ */
