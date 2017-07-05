//*****************************************************************************
//
// intc.c - Driver for the INTC.
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

#include "../inc/hw_intc.h"
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "intc.h"
#include "debug.h"

//*****************************************************************************
//
//! Enables an interrupt(unmask).
//!
//! \param ulInterrupt specifies the hardware interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.
//!
//! \return Return true if enable operation is success, otherwise false.
//
//*****************************************************************************
tBoolean
INTCInterruptEnable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
	    case IRQ_NO_DMIC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DMIC;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DMIC) != INTC_MR0_DMIC);
	    case IRQ_NO_AIC0 :
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_AIC0;
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_MR0_AIC0) != INTC_MR0_AIC0);
	    case IRQ_NO_BCH:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_BCH;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_BCH) != INTC_MR0_BCH);
	    case IRQ_NO_DSI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DSI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DSI) != INTC_MR0_DSI);
	    case IRQ_NO_CSI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_CSI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_CSI) != INTC_MR0_CSI);
	    case IRQ_NO_OHCI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_OHCI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_OHCI) != INTC_MR0_OHCI);
	    case IRQ_NO_IPU:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_IPU;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_IPU) != INTC_MR0_IPU);
	    case IRQ_NO_SSI1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SSI1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SSI1) != INTC_MR0_SSI1);
	    case IRQ_NO_SSI0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SSI0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SSI0) != INTC_MR0_SSI0);
	    case IRQ_NO_Reserved0:
		    break;
	    case IRQ_NO_PDMA:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_PDMA;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_PDMA) != INTC_MR0_PDMA);
	    case IRQ_NO_Reserved1:
		    break;
	    case IRQ_NO_GPIO5:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO5;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO5) != INTC_MR0_GPIO5);
	    case IRQ_NO_GPIO4:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO4;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO4) != INTC_MR0_GPIO4);
	    case IRQ_NO_GPIO3:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO3;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO3) != INTC_MR0_GPIO3);
	    case IRQ_NO_GPIO2:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO2;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO2) != INTC_MR0_GPIO2);
	    case IRQ_NO_GPIO1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO1) != INTC_MR0_GPIO1);
	    case IRQ_NO_GPIO0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO0) != INTC_MR0_GPIO0);
	    case IRQ_NO_SADC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SADC;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SADC) != INTC_MR0_SADC);
	    case IRQ_NO_EPDC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_EPDC;
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_MR0_EPDC) != INTC_MR0_EPDC);
	    case IRQ_NO_EHCI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_EHCI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_EHCI) != INTC_MR0_EHCI);
	    case IRQ_NO_OTG:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_OTG;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_OTG) != INTC_MR0_OTG);
	    case IRQ_NO_HASH:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_HASH;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_HASH) != INTC_MR0_HASH);
	    case IRQ_NO_AES:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_AES;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_AES) != INTC_MR0_AES);
	    case IRQ_NO_Reserved2:
		    break;
	    case IRQ_NO_TCU2:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU2;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU2) != INTC_MR0_TCU2);
	    case IRQ_NO_TCU1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU1) != INTC_MR0_TCU1);
	    case IRQ_NO_TCU0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU0) != INTC_MR0_TCU0);
	    case IRQ_NO_Reserved3:
		    break;
	    case IRQ_NO_ISP:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_ISP;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_ISP) != INTC_MR0_ISP);
	    case IRQ_NO_DELAY_LINE:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DELAY_LINE;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DELAY_LINE) != INTC_MR0_DELAY_LINE);
	    case IRQ_NO_LCD:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_LCD;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_LCD) != INTC_MR0_LCD);
	    case IRQ_NO_RTC:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_RTC;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_RTC) != INTC_MR1_RTC);
	    case IRQ_NO_Reserved4:
		    break;
	    case IRQ_NO_UART4:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART4;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART4) != INTC_MR1_UART4);
	    case IRQ_NO_MSC2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC2;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC2) != INTC_MR1_MSC2);
	    case IRQ_NO_MSC1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC1;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC1) != INTC_MR1_MSC1);
	    case IRQ_NO_MSC0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC0) != INTC_MR1_MSC0);
	    case IRQ_NO_Reserved5:
		    break;
	    case IRQ_NO_NFI:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_NFI;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_NFI) != INTC_MR1_NFI);
	    case IRQ_NO_PCM0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_PCM0;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_PCM0) != INTC_MR1_PCM0);
	    case IRQ_NO_Reserved6:
	    case IRQ_NO_Reserved7:
	    case IRQ_NO_Reserved8:
		    break;
	    case IRQ_NO_HARB2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB2;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB2) != INTC_MR1_HARB2);
	    case IRQ_NO_HARB1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB1;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB1) != INTC_MR1_HARB1);
	    case IRQ_NO_HARB0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB0;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB0) != INTC_MR1_HARB0);
	    case IRQ_NO_CPM:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_CPM;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_CPM) != INTC_MR1_CPM);
	    case IRQ_NO_UART3:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART3;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART3) != INTC_MR1_UART3);
	    case IRQ_NO_UART2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART2;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART2) != INTC_MR1_UART2);
	    case IRQ_NO_UART1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART1;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART1) != INTC_MR1_UART1);
	    case IRQ_NO_UART0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART0;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART0) != INTC_MR1_UART0);
	    case IRQ_NO_DDR:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_DDR;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_DDR) != INTC_MR1_DDR);
	    case IRQ_NO_Reserved9:
		    break;
	    case IRQ_NO_EFUSE:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_EFUSE;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_EFUSE) != INTC_MR1_EFUSE);
	    case IRQ_NO_ETHC:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_ETHC;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_ETHC) != INTC_MR1_ETHC);
	    case IRQ_NO_Reserved10:
		    break;
	    case IRQ_NO_I2C3:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C3;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C3) != INTC_MR1_I2C3);
	    case IRQ_NO_I2C2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C2;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C2) != INTC_MR1_I2C2);
	    case IRQ_NO_I2C1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C1;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C1) != INTC_MR1_I2C1);
	    case IRQ_NO_I2C0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C0;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C0) != INTC_MR1_I2C0);
	    case IRQ_NO_PDMAM:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_PDMAM;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_PDMAM) != INTC_MR1_PDMAM);
	    case IRQ_NO_VPU:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_VPU;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_VPU) != INTC_MR1_VPU);
	    case IRQ_NO_GPU:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_GPU;
		    return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_GPU) != INTC_MR1_GPU);
	    default:
		    break;
    }

    return(false);
}

//*****************************************************************************
//
//! Disables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.
//!
//! \return Return true if interrupt disable operation is success, otherwise false.
//
//*****************************************************************************
tBoolean
INTCInterruptDisable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
	    case IRQ_NO_DMIC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DMIC;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DMIC) == INTC_MR0_DMIC);
	    case IRQ_NO_AIC0 :
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_AIC0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_AIC0) == INTC_MR0_AIC0);
	    case IRQ_NO_BCH:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_BCH;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_BCH) == INTC_MR0_BCH);
	    case IRQ_NO_DSI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DSI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DSI) == INTC_MR0_DSI);
	    case IRQ_NO_CSI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_CSI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_CSI) == INTC_MR0_CSI);
	    case IRQ_NO_OHCI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_OHCI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_OHCI) == INTC_MR0_OHCI);
	    case IRQ_NO_IPU:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_IPU;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_IPU) == INTC_MR0_IPU);
	    case IRQ_NO_SSI1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SSI1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SSI1) == INTC_MR0_SSI1);
	    case IRQ_NO_SSI0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SSI0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SSI0) == INTC_MR0_SSI0);
	    case IRQ_NO_Reserved0:
		    break;
	    case IRQ_NO_PDMA:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_PDMA;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_PDMA) == INTC_MR0_PDMA);
	    case IRQ_NO_Reserved1:
		    break;
	    case IRQ_NO_GPIO5:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO5;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO5) == INTC_MR0_GPIO5);
	    case IRQ_NO_GPIO4:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO4;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO4) == INTC_MR0_GPIO4);
	    case IRQ_NO_GPIO3:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO3;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO3) == INTC_MR0_GPIO3);
	    case IRQ_NO_GPIO2:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO2;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO2) == INTC_MR0_GPIO2);
	    case IRQ_NO_GPIO1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO1) == INTC_MR0_GPIO1);
	    case IRQ_NO_GPIO0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_GPIO0) == INTC_MR0_GPIO0);
	    case IRQ_NO_SADC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SADC;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_SADC) == INTC_MR0_SADC);
	    case IRQ_NO_EPDC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_EPDC;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_EPDC) == INTC_MR0_EPDC);
	    case IRQ_NO_EHCI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_EHCI;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_EHCI) == INTC_MR0_EHCI);
	    case IRQ_NO_OTG:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_OTG;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_OTG) == INTC_MR0_OTG);
	    case IRQ_NO_HASH:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_HASH;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_HASH) == INTC_MR0_HASH);
	    case IRQ_NO_AES:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_AES;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_AES) == INTC_MR0_AES);
	    case IRQ_NO_Reserved2:
		    break;
	    case IRQ_NO_TCU2:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU2;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU2) == INTC_MR0_TCU2);
	    case IRQ_NO_TCU1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU1;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU1) == INTC_MR0_TCU1);
	    case IRQ_NO_TCU0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU0;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_TCU0) == INTC_MR0_TCU0);
	    case IRQ_NO_Reserved3:
		    break;
	    case IRQ_NO_ISP:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_ISP;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_ISP) == INTC_MR0_ISP);
	    case IRQ_NO_DELAY_LINE:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DELAY_LINE;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_DELAY_LINE) == INTC_MR0_DELAY_LINE);
	    case IRQ_NO_LCD:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_LCD;
            return((HWREG(INTC_BASE + INTC_O_MR0) & INTC_MR0_LCD) == INTC_MR0_LCD);
	    case IRQ_NO_RTC:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_RTC;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_RTC) == INTC_MR1_RTC);
	    case IRQ_NO_Reserved4:
		    break;
	    case IRQ_NO_UART4:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART4;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART4) == INTC_MR1_UART4);
	    case IRQ_NO_MSC2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC2;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC2) == INTC_MR1_MSC2);
	    case IRQ_NO_MSC1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC1;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC1) == INTC_MR1_MSC1);
	    case IRQ_NO_MSC0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_MSC0) == INTC_MR1_MSC0);
	    case IRQ_NO_Reserved5:
		    break;
	    case IRQ_NO_NFI:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_NFI;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_NFI) == INTC_MR1_NFI);
	    case IRQ_NO_PCM0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_PCM0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_PCM0) == INTC_MR1_PCM0);
	    case IRQ_NO_Reserved6:
	    case IRQ_NO_Reserved7:
	    case IRQ_NO_Reserved8:
		    break;
	    case IRQ_NO_HARB2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB2;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB2) == INTC_MR1_HARB2);
	    case IRQ_NO_HARB1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB1;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB1) == INTC_MR1_HARB1);
	    case IRQ_NO_HARB0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_HARB0) == INTC_MR1_HARB0);
	    case IRQ_NO_CPM:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_CPM;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_CPM) == INTC_MR1_CPM);
	    case IRQ_NO_UART3:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART3;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART3) == INTC_MR1_UART3);
	    case IRQ_NO_UART2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART2;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART2) == INTC_MR1_UART2);
	    case IRQ_NO_UART1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART1;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART1) == INTC_MR1_UART1);
	    case IRQ_NO_UART0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_UART0) == INTC_MR1_UART0);
	    case IRQ_NO_DDR:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_DDR;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_DDR) == INTC_MR1_DDR);
	    case IRQ_NO_Reserved9:
		    break;
	    case IRQ_NO_EFUSE:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_EFUSE;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_EFUSE) == INTC_MR1_EFUSE);
	    case IRQ_NO_ETHC:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_ETHC;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_ETHC) == INTC_MR1_ETHC);
	    case IRQ_NO_Reserved10:
		    break;
	    case IRQ_NO_I2C3:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C3;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C3) == INTC_MR1_I2C3);
	    case IRQ_NO_I2C2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C2;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C2) == INTC_MR1_I2C2);
	    case IRQ_NO_I2C1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C1;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C1) == INTC_MR1_I2C1);
	    case IRQ_NO_I2C0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C0;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_I2C0) == INTC_MR1_I2C0);
	    case IRQ_NO_PDMAM:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_PDMAM;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_PDMAM) == INTC_MR1_PDMAM);
	    case IRQ_NO_VPU:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_VPU;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_SR1_VPU) == INTC_SR1_VPU);
	    case IRQ_NO_GPU:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_GPU;
            return((HWREG(INTC_BASE + INTC_O_MR1) & INTC_MR1_GPU) == INTC_MR1_GPU);
	    default:
		    break;
    }
    
    return(false);
}

//*****************************************************************************
//
//! \internal
//! Print all register value.
//!
//! \param ulBase is the base address of the UART port.
//! \param print is a function pointer like printf.
//!
//! This function print value of all register in a UART port.
//!
//! \return none
//
//*****************************************************************************
void
INTCRegisterDump(int (*print)(const char *format, ...))
{
	REG_PRINT(INTC_O_SR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MSR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MCR0, INTC_BASE, print);
	REG_PRINT(INTC_O_PR0, INTC_BASE, print);
	REG_PRINT(INTC_O_SR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MSR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MCR1, INTC_BASE, print);
	REG_PRINT(INTC_O_PR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DSR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DMR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DPR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DSR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DMR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DPR1, INTC_BASE, print);
}

//*****************************************************************************
//
//! \Get INTC_O_SR0
//!
//! \param none.
//! \return Return the value of register INTC_O_SR0
//!
//*****************************************************************************
unsigned long
INTCSr0Get()
{
	return(HWREG(INTC_BASE + INTC_O_SR0));
}

//*****************************************************************************
//
//! \Get INTC_O_SR1
//!
//! \param none.
//! \return Return the value of INTC_O_SR1.
//!
//*****************************************************************************
unsigned long
INTCSr1Get()
{
	return(HWREG(INTC_BASE + INTC_O_SR1));
}

//*****************************************************************************
//
//! \Check an interrupt is pending or not
//!
//! \param ulInterrupt interrupt number.
//! \return Return true if the interrupt is pending, else false.
//!
//*****************************************************************************
tBoolean
INTCInterruptPending(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
        case IRQ_NO_DMIC:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_DMIC) == INTC_PR0_DMIC);
        case IRQ_NO_AIC0 :
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_PR0_AIC0) == INTC_PR0_AIC0);
        case IRQ_NO_BCH:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_BCH) == INTC_PR0_BCH);
        case IRQ_NO_DSI:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_DSI) == INTC_PR0_DSI);
        case IRQ_NO_CSI:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_CSI) == INTC_PR0_CSI);
        case IRQ_NO_OHCI:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_OHCI) == INTC_PR0_OHCI);
        case IRQ_NO_IPU:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_IPU) == INTC_PR0_IPU);
        case IRQ_NO_SSI1:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_SSI1) == INTC_PR0_SSI1);
        case IRQ_NO_SSI0:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_SSI0) == INTC_PR0_SSI0);
        case IRQ_NO_Reserved0:
            break;
        case IRQ_NO_PDMA:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_PDMA) == INTC_PR0_PDMA);
        case IRQ_NO_Reserved1:
            break;
        case IRQ_NO_GPIO5:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO5) == INTC_PR0_GPIO5);
        case IRQ_NO_GPIO4:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO4) == INTC_PR0_GPIO4);
        case IRQ_NO_GPIO3:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO3) == INTC_PR0_GPIO3);
        case IRQ_NO_GPIO2:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO2) == INTC_PR0_GPIO2);
        case IRQ_NO_GPIO1:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO1) == INTC_PR0_GPIO1);
        case IRQ_NO_GPIO0:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_GPIO0) == INTC_PR0_GPIO0);
        case IRQ_NO_SADC:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_SADC) == INTC_PR0_SADC);
        case IRQ_NO_EPDC:
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_PR0_EPDC) == INTC_PR0_EPDC);
        case IRQ_NO_EHCI:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_EHCI) == INTC_PR0_EHCI);
        case IRQ_NO_OTG:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_OTG) == INTC_PR0_OTG);
        case IRQ_NO_HASH:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_HASH) == INTC_PR0_HASH);
        case IRQ_NO_AES:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_AES) == INTC_PR0_AES);
        case IRQ_NO_Reserved2:
            break;
        case IRQ_NO_TCU2:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_TCU2) == INTC_PR0_TCU2);
        case IRQ_NO_TCU1:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_TCU1) == INTC_PR0_TCU1);
        case IRQ_NO_TCU0:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_TCU0) == INTC_PR0_TCU0);
        case IRQ_NO_Reserved3:
            break;
        case IRQ_NO_ISP:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_ISP) == INTC_PR0_ISP);
        case IRQ_NO_DELAY_LINE:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_DELAY_LINE) == INTC_PR0_DELAY_LINE);
        case IRQ_NO_LCD:
            return((HWREG(INTC_BASE + INTC_O_PR0) & INTC_PR0_LCD) == INTC_PR0_LCD);
        case IRQ_NO_RTC:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_RTC) == INTC_PR1_RTC);
        case IRQ_NO_Reserved4:
            break;
        case IRQ_NO_UART4:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_UART4) == INTC_PR1_UART4);
        case IRQ_NO_MSC2:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_MSC2) == INTC_PR1_MSC2);
        case IRQ_NO_MSC1:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_MSC1) == INTC_PR1_MSC1);
        case IRQ_NO_MSC0:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_MSC0) == INTC_PR1_MSC0);
        case IRQ_NO_Reserved5:
            break;
        case IRQ_NO_NFI:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_NFI) == INTC_PR1_NFI);
        case IRQ_NO_PCM0:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_PCM0) == INTC_PR1_PCM0);
        case IRQ_NO_Reserved6:
            break;
        case IRQ_NO_Reserved7:
            break;
        case IRQ_NO_Reserved8:
            break;
        case IRQ_NO_HARB2:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_HARB2) == INTC_PR1_HARB2);
        case IRQ_NO_HARB1:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_HARB1) == INTC_PR1_HARB1);
        case IRQ_NO_HARB0:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_HARB0) == INTC_PR1_HARB0);
        case IRQ_NO_CPM:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_CPM) == INTC_PR1_CPM);
        case IRQ_NO_UART3:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_UART3) == INTC_PR1_UART3);
        case IRQ_NO_UART2:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_UART2) == INTC_PR1_UART2);
        case IRQ_NO_UART1:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_UART1) == INTC_PR1_UART1);
        case IRQ_NO_UART0:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_UART0) == INTC_PR1_UART0);
        case IRQ_NO_DDR:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_DDR) == INTC_PR1_DDR);
        case IRQ_NO_Reserved9:
            break;
        case IRQ_NO_EFUSE:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_EFUSE) == INTC_PR1_EFUSE);
        case IRQ_NO_ETHC:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_ETHC) == INTC_PR1_ETHC);
        case IRQ_NO_Reserved10:
            break;
        case IRQ_NO_I2C3:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_I2C3) == INTC_PR1_I2C3);
        case IRQ_NO_I2C2:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_I2C2) == INTC_PR1_I2C2);
        case IRQ_NO_I2C1:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_I2C1) == INTC_PR1_I2C1);
        case IRQ_NO_I2C0:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_I2C0) == INTC_PR1_I2C0);
        case IRQ_NO_PDMAM:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_PDMAM) == INTC_PR1_PDMAM);
        case IRQ_NO_VPU:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_VPU) == INTC_PR1_VPU);
        case IRQ_NO_GPU:
            return((HWREG(INTC_BASE + INTC_O_PR1) & INTC_PR1_GPU) == INTC_PR1_GPU);
        default:
            break;
    }

    return(false);
}

//*****************************************************************************
//
//! \Get INTC_O_DSR0
//!
//! \param none.
//! \return Return the value of register INTC_O_DSR0
//!
//*****************************************************************************
unsigned long
INTCDMASr0Get()
{
	return(HWREG(INTC_BASE + INTC_O_DSR0));
}

//*****************************************************************************
//
//! \Get INTC_O_DSR1
//!
//! \param none.
//! \return Return the value of register INTC_O_DSR1
//!
//*****************************************************************************
unsigned long
INTCDMASr1Get()
{
	return(HWREG(INTC_BASE + INTC_O_DSR1));
}

//*****************************************************************************
//
//! Enables an DMA interrupt(unmask).
//!
//! \param ulInterrupt specifies the hardware interrupt to be enabled.
//!
//! The specified DMA interrupt is enabled in the interrupt controller.
//!
//! \return Return true if enable operation is success, otherwise false.
//
//*****************************************************************************
tBoolean
INTCDMAInterruptEnable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
        case IRQ_NO_DMIC:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_DMIC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DMIC) != INTC_DMR0_DMIC);
        case IRQ_NO_AIC0 :
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_AIC0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_AIC0) != INTC_DMR0_AIC0);
        case IRQ_NO_BCH:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_BCH;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_BCH) != INTC_DMR0_BCH);
        case IRQ_NO_DSI:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_DSI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DSI) != INTC_DMR0_DSI);
        case IRQ_NO_CSI:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_CSI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_CSI) != INTC_DMR0_CSI);
        case IRQ_NO_OHCI:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_OHCI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_OHCI) != INTC_DMR0_OHCI);
        case IRQ_NO_IPU:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_IPU;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_IPU) != INTC_DMR0_IPU);
        case IRQ_NO_SSI1:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_SSI1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SSI1) != INTC_DMR0_SSI1);
        case IRQ_NO_SSI0:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_SSI0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SSI0) != INTC_DMR0_SSI0);
        case IRQ_NO_Reserved0:
            break;
        case IRQ_NO_PDMA:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_PDMA;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_PDMA) != INTC_DMR0_PDMA);
        case IRQ_NO_Reserved1:
            break;
        case IRQ_NO_GPIO5:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO5;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO5) != INTC_DMR0_GPIO5);
        case IRQ_NO_GPIO4:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO4;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO4) != INTC_DMR0_GPIO4);
        case IRQ_NO_GPIO3:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO3;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO3) != INTC_DMR0_GPIO3);
        case IRQ_NO_GPIO2:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO2;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO2) != INTC_DMR0_GPIO2);
        case IRQ_NO_GPIO1:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO1) != INTC_DMR0_GPIO1);
        case IRQ_NO_GPIO0:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_GPIO0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO0) != INTC_DMR0_GPIO0);
        case IRQ_NO_SADC:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_SADC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SADC) != INTC_DMR0_SADC);
        case IRQ_NO_EPDC:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_EPDC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_EPDC) != INTC_DMR0_EPDC);
        case IRQ_NO_EHCI:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_EHCI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_EHCI) != INTC_DMR0_EHCI);
        case IRQ_NO_OTG:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_OTG;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_OTG) != INTC_DMR0_OTG);
        case IRQ_NO_HASH:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_HASH;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_HASH) != INTC_DMR0_HASH);
        case IRQ_NO_AES:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_AES;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_AES) != INTC_DMR0_AES);
        case IRQ_NO_Reserved2:
            break;
        case IRQ_NO_TCU2:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_TCU2;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU2) != INTC_DMR0_TCU2);
        case IRQ_NO_TCU1:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_TCU1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU1) != INTC_DMR0_TCU1);
        case IRQ_NO_TCU0:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_TCU0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU0) != INTC_DMR0_TCU0);
        case IRQ_NO_Reserved3:
            break;
        case IRQ_NO_ISP:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_ISP;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_ISP) != INTC_DMR0_ISP);
        case IRQ_NO_DELAY_LINE:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_DELAY_LINE;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DELAY_LINE) != INTC_DMR0_DELAY_LINE);
        case IRQ_NO_LCD:
            HWREG(INTC_BASE + INTC_O_DMR0) &= ~INTC_DMR0_LCD;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_LCD) != INTC_DMR0_LCD);
        case IRQ_NO_RTC:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_RTC;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_RTC) != INTC_DMR1_RTC);
        case IRQ_NO_Reserved4:
            break;
        case IRQ_NO_UART4:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_UART4;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART4) != INTC_DMR1_UART4);
        case IRQ_NO_MSC2:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_MSC2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC2) != INTC_DMR1_MSC2);
        case IRQ_NO_MSC1:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_MSC1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC1) != INTC_DMR1_MSC1);
        case IRQ_NO_MSC0:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_MSC0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC0) != INTC_DMR1_MSC0);
        case IRQ_NO_Reserved5:
            break;
        case IRQ_NO_NFI:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_NFI;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_NFI) != INTC_DMR1_NFI);
        case IRQ_NO_PCM0:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_PCM0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_PCM0) != INTC_DMR1_PCM0);
        case IRQ_NO_Reserved6:
        case IRQ_NO_Reserved7:
        case IRQ_NO_Reserved8:
            break;
        case IRQ_NO_HARB2:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_HARB2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB2) != INTC_DMR1_HARB2);
        case IRQ_NO_HARB1:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_HARB1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB1) != INTC_DMR1_HARB1);
        case IRQ_NO_HARB0:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_HARB0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB0) != INTC_DMR1_HARB0);
        case IRQ_NO_CPM:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_CPM;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_CPM) != INTC_DMR1_CPM);
        case IRQ_NO_UART3:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_UART3;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART3) != INTC_DMR1_UART3);
        case IRQ_NO_UART2:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_UART2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART2) != INTC_DMR1_UART2);
        case IRQ_NO_UART1:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_UART1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART1) != INTC_DMR1_UART1);
        case IRQ_NO_UART0:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_UART0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART0) != INTC_DMR1_UART0);
        case IRQ_NO_DDR:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_DDR;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_DDR) != INTC_DMR1_DDR);
        case IRQ_NO_Reserved9:
            break;
        case IRQ_NO_EFUSE:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_EFUSE;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_EFUSE) != INTC_DMR1_EFUSE);
        case IRQ_NO_ETHC:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_ETHC;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_ETHC) != INTC_DMR1_ETHC);
        case IRQ_NO_Reserved10:
            break;
        case IRQ_NO_I2C3:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_I2C3;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C3) != INTC_DMR1_I2C3);
        case IRQ_NO_I2C2:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_I2C2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C2) != INTC_DMR1_I2C2);
        case IRQ_NO_I2C1:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_I2C1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C1) != INTC_DMR1_I2C1);
        case IRQ_NO_I2C0:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_I2C0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C0) != INTC_DMR1_I2C0);
        case IRQ_NO_PDMAM:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_PDMAM;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_PDMAM) != INTC_DMR1_PDMAM);
        case IRQ_NO_VPU:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_VPU;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_VPU) != INTC_DMR1_VPU);
        case IRQ_NO_GPU:
            HWREG(INTC_BASE + INTC_O_DMR1) &= ~INTC_DMR1_GPU;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_GPU) != INTC_DMR1_GPU);
        default:
            break;
    }

    return(false);
}

//*****************************************************************************
//
//! Disables an DMA interrupt(mask).
//!
//! \param ulInterrupt specifies the DMA hardware interrupt to be enabled.
//!
//! The specified DMA interrupt is disabled in the interrupt controller.
//!
//! \return Return true if disable operation is success, otherwise false.
//
//*****************************************************************************
tBoolean
INTCDMAInterruptDisable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
        case IRQ_NO_DMIC:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_DMIC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DMIC) == INTC_DMR0_DMIC);
        case IRQ_NO_AIC0 :
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_AIC0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_AIC0) == INTC_DMR0_AIC0);
        case IRQ_NO_BCH:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_BCH;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_BCH) == INTC_DMR0_BCH);
        case IRQ_NO_DSI:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_DSI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DSI) == INTC_DMR0_DSI);
        case IRQ_NO_CSI:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_CSI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_CSI) == INTC_DMR0_CSI);
        case IRQ_NO_OHCI:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_OHCI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_OHCI) == INTC_DMR0_OHCI);
        case IRQ_NO_IPU:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_IPU;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_IPU) == INTC_DMR0_IPU);
        case IRQ_NO_SSI1:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_SSI1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SSI1) == INTC_DMR0_SSI1);
        case IRQ_NO_SSI0:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_SSI0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SSI0) == INTC_DMR0_SSI0);
        case IRQ_NO_Reserved0:
            break;
        case IRQ_NO_PDMA:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_PDMA;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_PDMA) == INTC_DMR0_PDMA);
        case IRQ_NO_Reserved1:
            break;
        case IRQ_NO_GPIO5:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO5;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO5) == INTC_DMR0_GPIO5);
        case IRQ_NO_GPIO4:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO4;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO4) == INTC_DMR0_GPIO4);
        case IRQ_NO_GPIO3:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO3;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO3) == INTC_DMR0_GPIO3);
        case IRQ_NO_GPIO2:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO2;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO2) == INTC_DMR0_GPIO2);
        case IRQ_NO_GPIO1:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO1) == INTC_DMR0_GPIO1);
        case IRQ_NO_GPIO0:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_GPIO0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_GPIO0) == INTC_DMR0_GPIO0);
        case IRQ_NO_SADC:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_SADC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_SADC) == INTC_DMR0_SADC);
        case IRQ_NO_EPDC:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_EPDC;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_EPDC) == INTC_DMR0_EPDC);
        case IRQ_NO_EHCI:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_EHCI;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_EHCI) == INTC_DMR0_EHCI);
        case IRQ_NO_OTG:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_OTG;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_OTG) == INTC_DMR0_OTG);
        case IRQ_NO_HASH:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_HASH;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_HASH) == INTC_DMR0_HASH);
        case IRQ_NO_AES:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_AES;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_AES) == INTC_DMR0_AES);
        case IRQ_NO_Reserved2:
            break;
        case IRQ_NO_TCU2:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_TCU2;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU2) == INTC_DMR0_TCU2);
        case IRQ_NO_TCU1:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_TCU1;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU1) == INTC_DMR0_TCU1);
        case IRQ_NO_TCU0:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_TCU0;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_TCU0) == INTC_DMR0_TCU0);
        case IRQ_NO_Reserved3:
            break;
        case IRQ_NO_ISP:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_ISP;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_ISP) == INTC_DMR0_ISP);
        case IRQ_NO_DELAY_LINE:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_DELAY_LINE;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_DELAY_LINE) == INTC_DMR0_DELAY_LINE);
        case IRQ_NO_LCD:
            HWREG(INTC_BASE + INTC_O_DMR0) |= INTC_DMR0_LCD;
            return((HWREG(INTC_BASE + INTC_O_DMR0) & INTC_DMR0_LCD) == INTC_DMR0_LCD);
        case IRQ_NO_RTC:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_RTC;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_RTC) == INTC_DMR1_RTC);
        case IRQ_NO_Reserved4:
            break;
        case IRQ_NO_UART4:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_UART4;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART4) == INTC_DMR1_UART4);
        case IRQ_NO_MSC2:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_MSC2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC2) == INTC_DMR1_MSC2);
        case IRQ_NO_MSC1:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_MSC1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC1) == INTC_DMR1_MSC1);
        case IRQ_NO_MSC0:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_MSC0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_MSC0) == INTC_DMR1_MSC0);
        case IRQ_NO_Reserved5:
            break;
        case IRQ_NO_NFI:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_NFI;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_NFI) == INTC_DMR1_NFI);
        case IRQ_NO_PCM0:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_PCM0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_PCM0) == INTC_DMR1_PCM0);
        case IRQ_NO_Reserved6:
        case IRQ_NO_Reserved7:
        case IRQ_NO_Reserved8:
            break;
        case IRQ_NO_HARB2:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_HARB2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB2) == INTC_DMR1_HARB2);
        case IRQ_NO_HARB1:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_HARB1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB1) == INTC_DMR1_HARB1);
        case IRQ_NO_HARB0:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_HARB0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_HARB0) == INTC_DMR1_HARB0);
        case IRQ_NO_CPM:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_CPM;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_CPM) == INTC_DMR1_CPM);
        case IRQ_NO_UART3:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_UART3;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART3) == INTC_DMR1_UART3);
        case IRQ_NO_UART2:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_UART2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART2) == INTC_DMR1_UART2);
        case IRQ_NO_UART1:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_UART1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART1) == INTC_DMR1_UART1);
        case IRQ_NO_UART0:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_UART0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_UART0) == INTC_DMR1_UART0);
        case IRQ_NO_DDR:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_DDR;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_DDR) == INTC_DMR1_DDR);
        case IRQ_NO_Reserved9:
            break;
        case IRQ_NO_EFUSE:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_EFUSE;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_EFUSE) == INTC_DMR1_EFUSE);
        case IRQ_NO_ETHC:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_ETHC;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_ETHC) == INTC_DMR1_ETHC);
        case IRQ_NO_Reserved10:
            break;
        case IRQ_NO_I2C3:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_I2C3;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C3) == INTC_DMR1_I2C3);
        case IRQ_NO_I2C2:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_I2C2;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C2) == INTC_DMR1_I2C2);
        case IRQ_NO_I2C1:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_I2C1;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C1) == INTC_DMR1_I2C1);
        case IRQ_NO_I2C0:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_I2C0;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_I2C0) == INTC_DMR1_I2C0);
        case IRQ_NO_PDMAM:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_PDMAM;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_PDMAM) == INTC_DMR1_PDMAM);
        case IRQ_NO_VPU:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_VPU;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_VPU) == INTC_DMR1_VPU);
        case IRQ_NO_GPU:
            HWREG(INTC_BASE + INTC_O_DMR1) |= INTC_DMR1_GPU;
            return((HWREG(INTC_BASE + INTC_O_DMR1) & INTC_DMR1_GPU) == INTC_DMR1_GPU);
        default:
            break;
    }

    return(false);
}

//*****************************************************************************
//
//! \Check a DMA interrupt is pending or not
//! a DMA interrupt is a subinterrupt of PDMA(IRQ_NO_PDMA)
//!
//! \param ulInterrupt interrupt number.
//! \return Return true if the DMA interrupt is pending, else false.
//!
//*****************************************************************************
tBoolean
INTCDMAInterruptPending(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUM);

    switch (ulInterrupt) {
        case IRQ_NO_DMIC:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_DMIC) == INTC_DPR0_DMIC);
        case IRQ_NO_AIC0 :
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_DPR0_AIC0) == INTC_DPR0_AIC0);
        case IRQ_NO_BCH:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_BCH) == INTC_DPR0_BCH);
        case IRQ_NO_DSI:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_DSI) == INTC_DPR0_DSI);
        case IRQ_NO_CSI:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_CSI) == INTC_DPR0_CSI);
        case IRQ_NO_OHCI:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_OHCI) == INTC_DPR0_OHCI);
        case IRQ_NO_IPU:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_IPU) == INTC_DPR0_IPU);
        case IRQ_NO_SSI1:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_SSI1) == INTC_DPR0_SSI1);
        case IRQ_NO_SSI0:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_SSI0) == INTC_DPR0_SSI0);
        case IRQ_NO_Reserved0:
            break;
        case IRQ_NO_PDMA:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_PDMA) == INTC_DPR0_PDMA);
        case IRQ_NO_Reserved1:
            break;
        case IRQ_NO_GPIO5:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO5) == INTC_DPR0_GPIO5);
        case IRQ_NO_GPIO4:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO4) == INTC_DPR0_GPIO4);
        case IRQ_NO_GPIO3:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO3) == INTC_DPR0_GPIO3);
        case IRQ_NO_GPIO2:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO2) == INTC_DPR0_GPIO2);
        case IRQ_NO_GPIO1:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO1) == INTC_DPR0_GPIO1);
        case IRQ_NO_GPIO0:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_GPIO0) == INTC_DPR0_GPIO0);
        case IRQ_NO_SADC:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_SADC) == INTC_DPR0_SADC);
        case IRQ_NO_EPDC:
            return((HWREG(INTC_BASE + INTC_O_MCR0) & INTC_DPR0_EPDC) == INTC_DPR0_EPDC);
        case IRQ_NO_EHCI:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_EHCI) == INTC_DPR0_EHCI);
        case IRQ_NO_OTG:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_OTG) == INTC_DPR0_OTG);
        case IRQ_NO_HASH:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_HASH) == INTC_DPR0_HASH);
        case IRQ_NO_AES:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_AES) == INTC_DPR0_AES);
        case IRQ_NO_Reserved2:
            break;
        case IRQ_NO_TCU2:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_TCU2) == INTC_DPR0_TCU2);
        case IRQ_NO_TCU1:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_TCU1) == INTC_DPR0_TCU1);
        case IRQ_NO_TCU0:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_TCU0) == INTC_DPR0_TCU0);
        case IRQ_NO_Reserved3:
            break;
        case IRQ_NO_ISP:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_ISP) == INTC_DPR0_ISP);
        case IRQ_NO_DELAY_LINE:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_DELAY_LINE) == INTC_DPR0_DELAY_LINE);
        case IRQ_NO_LCD:
            return((HWREG(INTC_BASE +INTC_O_DPR0) & INTC_DPR0_LCD) == INTC_DPR0_LCD);
        case IRQ_NO_RTC:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_RTC) == INTC_DMR1_RTC);
        case IRQ_NO_Reserved4:
            break;
        case IRQ_NO_UART4:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_UART4) == INTC_DMR1_UART4);
        case IRQ_NO_MSC2:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_MSC2) == INTC_DMR1_MSC2);
        case IRQ_NO_MSC1:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_MSC1) == INTC_DMR1_MSC1);
        case IRQ_NO_MSC0:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_MSC0) == INTC_DMR1_MSC0);
        case IRQ_NO_Reserved5:
            break;
        case IRQ_NO_NFI:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_NFI) == INTC_DMR1_NFI);
        case IRQ_NO_PCM0:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_PCM0) == INTC_DMR1_PCM0);
        case IRQ_NO_Reserved6:
            break;
        case IRQ_NO_Reserved7:
            break;
        case IRQ_NO_Reserved8:
            break;
        case IRQ_NO_HARB2:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_HARB2) == INTC_DMR1_HARB2);
        case IRQ_NO_HARB1:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_HARB1) == INTC_DMR1_HARB1);
        case IRQ_NO_HARB0:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_HARB0) == INTC_DMR1_HARB0);
        case IRQ_NO_CPM:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_CPM) == INTC_DMR1_CPM);
        case IRQ_NO_UART3:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_UART3) == INTC_DMR1_UART3);
        case IRQ_NO_UART2:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_UART2) == INTC_DMR1_UART2);
        case IRQ_NO_UART1:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_UART1) == INTC_DMR1_UART1);
        case IRQ_NO_UART0:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_UART0) == INTC_DMR1_UART0);
        case IRQ_NO_DDR:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_DDR) == INTC_DMR1_DDR);
        case IRQ_NO_Reserved9:
            break;
        case IRQ_NO_EFUSE:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_EFUSE) == INTC_DMR1_EFUSE);
        case IRQ_NO_ETHC:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_ETHC) == INTC_DMR1_ETHC);
        case IRQ_NO_Reserved10:
            break;
        case IRQ_NO_I2C3:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_I2C3) == INTC_DMR1_I2C3);
        case IRQ_NO_I2C2:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_I2C2) == INTC_DMR1_I2C2);
        case IRQ_NO_I2C1:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_I2C1) == INTC_DMR1_I2C1);
        case IRQ_NO_I2C0:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_I2C0) == INTC_DMR1_I2C0);
        case IRQ_NO_PDMAM:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_PDMAM) == INTC_DMR1_PDMAM);
        case IRQ_NO_VPU:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_VPU) == INTC_DMR1_VPU);
        case IRQ_NO_GPU:
            return((HWREG(INTC_BASE + INTC_O_DPR1) & INTC_DMR1_GPU) == INTC_DMR1_GPU);
        default:
            break;
    }

    return(false);
}
