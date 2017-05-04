//*****************************************************************************
//
// hw_intc.h - Macros used when accessing the Interrupt Controller hardware.
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
#ifndef __HW_INTC_H__
#define __HW_INTC_H__

//*****************************************************************************
//
// The following are defines for the INTC address and interrupt number.
//
//*****************************************************************************
#define INTC_ADDRESS            0xB0001000  // Address of INTC
#define INTC_INTERRUPT_NUM      0x00000040  // Number of interrupts

//*****************************************************************************
//
// The following are defines for the INTC register offsets.
//
//*****************************************************************************
#define INTC_O_SR0              0x00000000  // INTC Source Register0
#define INTC_O_MR0              0x00000004  // INTC Mask Register0
#define INTC_O_MSR0             0x00000008  // INTC Mask Set Register0
#define INTC_O_MCR0             0x0000000C  // INTC Mask Clear Register0
#define INTC_O_PR0              0x00000010  // INTC Pending Register0
#define INTC_O_SR1              0x00000020  // INTC Source Register1
#define INTC_O_MR1              0x00000024  // INTC Mask Register1
#define INTC_O_MSR1             0x00000028  // INTC Mask Set Register1
#define INTC_O_MCR1             0x0000002C  // INTC Mask Clear Register1
#define INTC_O_PR1              0x00000030  // INTC Pending Register1
#define INTC_O_DSR0             0x00000034  // INTC Source Register0 for PDMA
#define INTC_O_DMR0             0x00000038  // INTC Mask Register0 for PDMA
#define INTC_O_DPR0             0x0000003C  // INTC Pending Register0 for PDMA
#define INTC_O_DSR1             0x00000040  // INTC Source Register1 for PDMA
#define INTC_O_DMR1             0x00000044  // INTC Mask Register1 for PDMA
#define INTC_O_DPR1             0x00000048  // INTC Pending Register1 for PDMA

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_SR0 register.
// 1: pending, 0: not pending
//
//*****************************************************************************
#define INTC_SR0_LCD            0x80000000  // LCD interrupt pending
#define INTC_SR0_DELAY_LINE     0x40000000  // DELAY_LINE interrupt pending
#define INTC_SR0_ISP            0x20000000  // ISP interrupt pending
#define INTC_SR0_TCU0           0x08000000  // TCU0 interrupt pending
#define INTC_SR0_TCU1           0x04000000  // TCU1 interrupt pending
#define INTC_SR0_TCU2           0x02000000  // TCU2 interrupt pending
#define INTC_SR0_AES            0x00800000  // AES interrupt pending
#define INTC_SR0_HASH           0x00400000  // HASH interrupt pending
#define INTC_SR0_OTG            0x00200000  // OTG interrupt pending
#define INTC_SR0_EHCI           0x00100000  // EHCI interrupt pending
#define INTC_SR0_EPDC           0x00080000  // EPDC interrupt pending
#define INTC_SR0_SADC           0x00040000  // SADC interrupt pending
#define INTC_SR0_GPIO0          0x00020000  // GPIO0 interrupt pending
#define INTC_SR0_GPIO1          0x00010000  // GPIO1 interrupt pending
#define INTC_SR0_GPIO2          0x00008000  // GPIO2 interrupt pending
#define INTC_SR0_GPIO3          0x00004000  // GPIO3 interrupt pending
#define INTC_SR0_GPIO4          0x00002000  // GPIO4 interrupt pending
#define INTC_SR0_GPIO5          0x00001000  // GPIO5 interrupt pending
#define INTC_SR0_PDMA           0x00000400  // PDMA interrupt pending
#define INTC_SR0_SSI0           0x00000100  // SSI0 interrupt pending
#define INTC_SR0_SSI1           0x00000080  // SSI1 interrupt pending
#define INTC_SR0_IPU            0x00000040  // IPU interrupt pending
#define INTC_SR0_OHCI           0x00000020  // OHCI interrupt pending
#define INTC_SR0_CSI            0x00000010  // CSI interrupt pending
#define INTC_SR0_DSI            0x00000008  // DSI interrupt pending
#define INTC_SR0_BCH            0x00000004  // BCH interrupt pending
#define INTC_SR0_AIC0           0x00000002  // AIC0 interrupt pending
#define INTC_SR0_DMIC           0x00000001  // DMIC interrupt pending

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MR0 register.
// 1: masked, 0: not masked
//
//*****************************************************************************
#define INTC_MR0_LCD            0x80000000  // LCD interrupt masked
#define INTC_MR0_DELAY_LINE     0x40000000  // DELAY_LINE interrupt masked
#define INTC_MR0_ISP            0x20000000  // ISP interrupt masked
#define INTC_MR0_TCU0           0x08000000  // TCU0 interrupt masked
#define INTC_MR0_TCU1           0x04000000  // TCU1 interrupt masked
#define INTC_MR0_TCU2           0x02000000  // TCU2 interrupt masked
#define INTC_MR0_AES            0x00800000  // AES interrupt masked
#define INTC_MR0_HASH           0x00400000  // HASH interrupt masked
#define INTC_MR0_OTG            0x00200000  // OTG interrupt masked
#define INTC_MR0_EHCI           0x00100000  // EHCI interrupt masked
#define INTC_MR0_EPDC           0x00080000  // EPDC interrupt masked
#define INTC_MR0_SADC           0x00040000  // SADC interrupt masked
#define INTC_MR0_GPIO0          0x00020000  // GPIO0 interrupt masked
#define INTC_MR0_GPIO1          0x00010000  // GPIO1 interrupt masked
#define INTC_MR0_GPIO2          0x00008000  // GPIO2 interrupt masked
#define INTC_MR0_GPIO3          0x00004000  // GPIO3 interrupt masked
#define INTC_MR0_GPIO4          0x00002000  // GPIO4 interrupt masked
#define INTC_MR0_GPIO5          0x00001000  // GPIO5 interrupt masked
#define INTC_MR0_PDMA           0x00000400  // PDMA interrupt masked
#define INTC_MR0_SSI0           0x00000100  // SSI0 interrupt masked
#define INTC_MR0_SSI1           0x00000080  // SSI1 interrupt masked
#define INTC_MR0_IPU            0x00000040  // IPU interrupt masked
#define INTC_MR0_OHCI           0x00000020  // OHCI interrupt masked
#define INTC_MR0_CSI            0x00000010  // CSI interrupt masked
#define INTC_MR0_DSI            0x00000008  // DSI interrupt masked
#define INTC_MR0_BCH            0x00000004  // BCH interrupt masked
#define INTC_MR0_AIC0           0x00000002  // AIC0 interrupt masked
#define INTC_MR0_DMIC           0x00000001  // DMIC interrupt masked

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MSR0 register.
// 1: set the corresponding interrupt mask bit, 0: ignore 
//
//*****************************************************************************
#define INTC_MSR0_LCD            0x80000000  // LCD interrupt set mask bit
#define INTC_MSR0_DELAY_LINE     0x40000000  // DELAY_LINE interrupt set mask bit
#define INTC_MSR0_ISP            0x20000000  // ISP interrupt set mask bit
#define INTC_MSR0_TCU0           0x08000000  // TCU0 interrupt set mask bit
#define INTC_MSR0_TCU1           0x04000000  // TCU1 interrupt set mask bit
#define INTC_MSR0_TCU2           0x02000000  // TCU2 interrupt set mask bit
#define INTC_MSR0_AES            0x00800000  // AES interrupt set mask bit
#define INTC_MSR0_HASH           0x00400000  // HASH interrupt set mask bit
#define INTC_MSR0_OTG            0x00200000  // OTG interrupt set mask bit
#define INTC_MSR0_EHCI           0x00100000  // EHCI interrupt set mask bit
#define INTC_MSR0_EPDC           0x00080000  // EPDC interrupt set mask bit
#define INTC_MSR0_SADC           0x00040000  // SADC interrupt set mask bit
#define INTC_MSR0_GPIO0          0x00020000  // GPIO0 interrupt set mask bit
#define INTC_MSR0_GPIO1          0x00010000  // GPIO1 interrupt set mask bit
#define INTC_MSR0_GPIO2          0x00008000  // GPIO2 interrupt set mask bit
#define INTC_MSR0_GPIO3          0x00004000  // GPIO3 interrupt set mask bit
#define INTC_MSR0_GPIO4          0x00002000  // GPIO4 interrupt set mask bit
#define INTC_MSR0_GPIO5          0x00001000  // GPIO5 interrupt set mask bit
#define INTC_MSR0_PDMA           0x00000400  // PDMA interrupt set mask bit
#define INTC_MSR0_SSI0           0x00000100  // SSI0 interrupt set mask bit
#define INTC_MSR0_SSI1           0x00000080  // SSI1 interrupt set mask bit
#define INTC_MSR0_IPU            0x00000040  // IPU interrupt set mask bit
#define INTC_MSR0_OHCI           0x00000020  // OHCI interrupt set mask bit
#define INTC_MSR0_CSI            0x00000010  // CSI interrupt set mask bit
#define INTC_MSR0_DSI            0x00000008  // DSI interrupt set mask bit
#define INTC_MSR0_BCH            0x00000004  // BCH interrupt set mask bit
#define INTC_MSR0_AIC0           0x00000002  // AIC0 interrupt set mask bit
#define INTC_MSR0_DMIC           0x00000001  // DMIC interrupt set mask bit

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MCR0 register.
// 1: clear the corresponding interrupt mask bit, 0: ignore
//
//*****************************************************************************
#define INTC_MCR0_LCD            0x80000000  // LCD interrupt clear mask bit
#define INTC_MCR0_DELAY_LINE     0x40000000  // DELAY_LINE interrupt clear mask bit
#define INTC_MCR0_ISP            0x20000000  // ISP interrupt clear mask bit
#define INTC_MCR0_TCU0           0x08000000  // TCU0 interrupt clear mask bit
#define INTC_MCR0_TCU1           0x04000000  // TCU1 interrupt clear mask bit
#define INTC_MCR0_TCU2           0x02000000  // TCU2 interrupt clear mask bit
#define INTC_MCR0_AES            0x00800000  // AES interrupt clear mask bit
#define INTC_MCR0_HASH           0x00400000  // HASH interrupt clear mask bit
#define INTC_MCR0_OTG            0x00200000  // OTG interrupt clear mask bit
#define INTC_MCR0_EHCI           0x00100000  // EHCI interrupt clear mask bit
#define INTC_MCR0_EPDC           0x00080000  // EPDC interrupt clear mask bit
#define INTC_MCR0_SADC           0x00040000  // SADC interrupt clear mask bit
#define INTC_MCR0_GPIO0          0x00020000  // GPIO0 interrupt clear mask bit
#define INTC_MCR0_GPIO1          0x00010000  // GPIO1 interrupt clear mask bit
#define INTC_MCR0_GPIO2          0x00008000  // GPIO2 interrupt clear mask bit
#define INTC_MCR0_GPIO3          0x00004000  // GPIO3 interrupt clear mask bit
#define INTC_MCR0_GPIO4          0x00002000  // GPIO4 interrupt clear mask bit
#define INTC_MCR0_GPIO5          0x00001000  // GPIO5 interrupt clear mask bit
#define INTC_MCR0_PDMA           0x00000400  // PDMA interrupt clear mask bit
#define INTC_MCR0_SSI0           0x00000100  // SSI0 interrupt clear mask bit
#define INTC_MCR0_SSI1           0x00000080  // SSI1 interrupt clear mask bit
#define INTC_MCR0_IPU            0x00000040  // IPU interrupt clear mask bit
#define INTC_MCR0_OHCI           0x00000020  // OHCI interrupt clear mask bit
#define INTC_MCR0_CSI            0x00000010  // CSI interrupt clear mask bit
#define INTC_MCR0_DSI            0x00000008  // DSI interrupt clear mask bit
#define INTC_MCR0_BCH            0x00000004  // BCH interrupt clear mask bit
#define INTC_MCR0_AIC0           0x00000002  // AIC0 interrupt clear mask bit
#define INTC_MCR0_DMIC           0x00000001  // DMIC interrupt clear mask bit

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_PR0 register.
// 0: the corresponding interrupt is not active or is masked.
// 1: the corresponding interrupt is active or is not masked to the processor.
//
//*****************************************************************************
#define INTC_PR0_LCD            0x80000000  // LCD interrupt active/mask status
#define INTC_PR0_DELAY_LINE     0x40000000  // DELAY_LINE interrupt active/mask status
#define INTC_PR0_ISP            0x20000000  // ISP interrupt active/mask status
#define INTC_PR0_TCU0           0x08000000  // TCU0 interrupt active/mask status
#define INTC_PR0_TCU1           0x04000000  // TCU1 interrupt active/mask status
#define INTC_PR0_TCU2           0x02000000  // TCU2 interrupt active/mask status
#define INTC_PR0_AES            0x00800000  // AES interrupt active/mask status
#define INTC_PR0_HASH           0x00400000  // HASH interrupt active/mask status
#define INTC_PR0_OTG            0x00200000  // OTG interrupt active/mask status
#define INTC_PR0_EHCI           0x00100000  // EHCI interrupt active/mask status
#define INTC_PR0_EPDC           0x00080000  // EPDC interrupt active/mask status
#define INTC_PR0_SADC           0x00040000  // SADC interrupt active/mask status
#define INTC_PR0_GPIO0          0x00020000  // GPIO0 interrupt active/mask status
#define INTC_PR0_GPIO1          0x00010000  // GPIO1 interrupt active/mask status
#define INTC_PR0_GPIO2          0x00008000  // GPIO2 interrupt active/mask status
#define INTC_PR0_GPIO3          0x00004000  // GPIO3 interrupt active/mask status
#define INTC_PR0_GPIO4          0x00002000  // GPIO4 interrupt active/mask status
#define INTC_PR0_GPIO5          0x00001000  // GPIO5 interrupt active/mask status
#define INTC_PR0_PDMA           0x00000400  // PDMA interrupt active/mask status
#define INTC_PR0_SSI0           0x00000100  // SSI0 interrupt active/mask status
#define INTC_PR0_SSI1           0x00000080  // SSI1 interrupt active/mask status
#define INTC_PR0_IPU            0x00000040  // IPU interrupt active/mask status
#define INTC_PR0_OHCI           0x00000020  // OHCI interrupt active/mask status
#define INTC_PR0_CSI            0x00000010  // CSI interrupt active/mask status
#define INTC_PR0_DSI            0x00000008  // DSI interrupt active/mask status
#define INTC_PR0_BCH            0x00000004  // BCH interrupt active/mask status
#define INTC_PR0_AIC0           0x00000002  // AIC0 interrupt active/mask status
#define INTC_PR0_DMIC           0x00000001  // DMIC interrupt active/mask status

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_SR1 register.
// 1: pending, 0: not pending
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_SR1_GPU            0x80000000  // GPU interrupt pending
#define INTC_SR1_VPU            0x40000000  // VPU interrupt pending
#define INTC_SR1_PDMAM          0x20000000  // PDMAM interrupt pending
#define INTC_SR1_I2C0           0x10000000  // I2C0 interrupt pending
#define INTC_SR1_I2C1           0x08000000  // I2C1 interrupt pending
#define INTC_SR1_I2C2           0x04000000  // I2C2 interrupt pending
#define INTC_SR1_I2C3           0x02000000  // I2C3 interrupt pending
#define INTC_SR1_ETHC           0x00800000  // ETHC interrupt pending
#define INTC_SR1_EFUSE          0x00400000  // EFUSE interrupt pending
#define INTC_SR1_DDR            0x00100000  // DDR interrupt pending
#define INTC_SR1_UART0          0x00080000  // UART0 interrupt pending
#define INTC_SR1_UART1          0x00040000  // UART1 interrupt pending
#define INTC_SR1_UART2          0x00020000  // UART2 interrupt pending
#define INTC_SR1_UART3          0x00010000  // UART3 interrupt pending
#define INTC_SR1_CPM            0x00008000  // CPM interrupt pending
#define INTC_SR1_HARB0          0x00004000  // HARB0 interrupt pending
#define INTC_SR1_HARB1          0x00002000  // HARB1 interrupt pending
#define INTC_SR1_HARB2          0x00001000  // HARB2 interrupt pending
#define INTC_SR1_PCM0           0x00000100  // PCM0 interrupt pending
#define INTC_SR1_NFI            0x00000080  // NFI interrupt pending
#define INTC_SR1_MSC0           0x00000020  // MSC0 interrupt pending
#define INTC_SR1_MSC1           0x00000010  // MSC1 interrupt pending
#define INTC_SR1_MSC2           0x00000008  // MSC1 interrupt pending
#define INTC_SR1_UART4          0x00000004  // UART4 interrupt pending
#define INTC_SR1_RTC            0x00000001  // RTC interrupt pending

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MR1 register.
// 1: masked, 0: not masked
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_MR1_GPU            0x80000000  // GPU interrupt masked
#define INTC_MR1_VPU            0x40000000  // VPU interrupt masked
#define INTC_MR1_PDMAM          0x20000000  // PDMAM interrupt masked
#define INTC_MR1_I2C0           0x10000000  // I2C0 interrupt masked
#define INTC_MR1_I2C1           0x08000000  // I2C1 interrupt masked
#define INTC_MR1_I2C2           0x04000000  // I2C2 interrupt masked
#define INTC_MR1_I2C3           0x02000000  // I2C3 interrupt masked
#define INTC_MR1_ETHC           0x00800000  // ETHC interrupt masked
#define INTC_MR1_EFUSE          0x00400000  // EFUSE interrupt masked
#define INTC_MR1_DDR            0x00100000  // DDR interrupt masked
#define INTC_MR1_UART0          0x00080000  // UART0 interrupt masked
#define INTC_MR1_UART1          0x00040000  // UART1 interrupt masked
#define INTC_MR1_UART2          0x00020000  // UART2 interrupt masked
#define INTC_MR1_UART3          0x00010000  // UART3 interrupt masked
#define INTC_MR1_CPM            0x00008000  // CPM interrupt masked
#define INTC_MR1_HARB0          0x00004000  // HARB0 interrupt masked
#define INTC_MR1_HARB1          0x00002000  // HARB1 interrupt masked
#define INTC_MR1_HARB2          0x00001000  // HARB2 interrupt masked
#define INTC_MR1_PCM0           0x00000100  // PCM0 interrupt masked
#define INTC_MR1_NFI            0x00000080  // NFI interrupt masked
#define INTC_MR1_MSC0           0x00000020  // MSC0 interrupt masked
#define INTC_MR1_MSC1           0x00000010  // MSC1 interrupt masked
#define INTC_MR1_MSC2           0x00000008  // MSC1 interrupt masked
#define INTC_MR1_UART4          0x00000004  // UART4 interrupt masked
#define INTC_MR1_RTC            0x00000001  // RTC interrupt masked

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MSR1 register.
// 1: set the corresponding interrupt mask bit, 0: ignore 
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_MSR1_GPU            0x80000000  // GPU interrupt set mask bit
#define INTC_MSR1_VPU            0x40000000  // VPU interrupt set mask bit
#define INTC_MSR1_PDMAM          0x20000000  // PDMAM interrupt set mask bit
#define INTC_MSR1_I2C0           0x10000000  // I2C0 interrupt set mask bit
#define INTC_MSR1_I2C1           0x08000000  // I2C1 interrupt set mask bit
#define INTC_MSR1_I2C2           0x04000000  // I2C2 interrupt set mask bit
#define INTC_MSR1_I2C3           0x02000000  // I2C3 interrupt set mask bit
#define INTC_MSR1_ETHC           0x00800000  // ETHC interrupt set mask bit
#define INTC_MSR1_EFUSE          0x00400000  // EFUSE interrupt set mask bit
#define INTC_MSR1_DDR            0x00100000  // DDR interrupt set mask bit
#define INTC_MSR1_UART0          0x00080000  // UART0 interrupt set mask bit
#define INTC_MSR1_UART1          0x00040000  // UART1 interrupt set mask bit
#define INTC_MSR1_UART2          0x00020000  // UART2 interrupt set mask bit
#define INTC_MSR1_UART3          0x00010000  // UART3 interrupt set mask bit
#define INTC_MSR1_CPM            0x00008000  // CPM interrupt set mask bit
#define INTC_MSR1_HARB0          0x00004000  // HARB0 interrupt set mask bit
#define INTC_MSR1_HARB1          0x00002000  // HARB1 interrupt set mask bit
#define INTC_MSR1_HARB2          0x00001000  // HARB2 interrupt set mask bit
#define INTC_MSR1_PCM0           0x00000100  // PCM0 interrupt set mask bit
#define INTC_MSR1_NFI            0x00000080  // NFI interrupt set mask bit
#define INTC_MSR1_MSC0           0x00000020  // MSC0 interrupt set mask bit
#define INTC_MSR1_MSC1           0x00000010  // MSC1 interrupt set mask bit
#define INTC_MSR1_MSC2           0x00000008  // MSC1 interrupt set mask bit
#define INTC_MSR1_UART4          0x00000004  // UART4 interrupt set mask bit
#define INTC_MSR1_RTC            0x00000001  // RTC interrupt set mask bit

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_MCR1 register.
// 1: clear the corresponding interrupt mask bit, 0: ignore 
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_MCR1_GPU            0x80000000  // GPU interrupt clear mask bit
#define INTC_MCR1_VPU            0x40000000  // VPU interrupt clear mask bit
#define INTC_MCR1_PDMAM          0x20000000  // PDMAM interrupt clear mask bit
#define INTC_MCR1_I2C0           0x10000000  // I2C0 interrupt clear mask bit
#define INTC_MCR1_I2C1           0x08000000  // I2C1 interrupt clear mask bit
#define INTC_MCR1_I2C2           0x04000000  // I2C2 interrupt clear mask bit
#define INTC_MCR1_I2C3           0x02000000  // I2C3 interrupt clear mask bit
#define INTC_MCR1_ETHC           0x00800000  // ETHC interrupt clear mask bit
#define INTC_MCR1_EFUSE          0x00400000  // EFUSE interrupt clear mask bit
#define INTC_MCR1_DDR            0x00100000  // DDR interrupt clear mask bit
#define INTC_MCR1_UART0          0x00080000  // UART0 interrupt clear mask bit
#define INTC_MCR1_UART1          0x00040000  // UART1 interrupt clear mask bit
#define INTC_MCR1_UART2          0x00020000  // UART2 interrupt clear mask bit
#define INTC_MCR1_UART3          0x00010000  // UART3 interrupt clear mask bit
#define INTC_MCR1_CPM            0x00008000  // CPM interrupt clear mask bit
#define INTC_MCR1_HARB0          0x00004000  // HARB0 interrupt clear mask bit
#define INTC_MCR1_HARB1          0x00002000  // HARB1 interrupt clear mask bit
#define INTC_MCR1_HARB2          0x00001000  // HARB2 interrupt clear mask bit
#define INTC_MCR1_PCM0           0x00000100  // PCM0 interrupt clear mask bit
#define INTC_MCR1_NFI            0x00000080  // NFI interrupt clear mask bit
#define INTC_MCR1_MSC0           0x00000020  // MSC0 interrupt clear mask bit
#define INTC_MCR1_MSC1           0x00000010  // MSC1 interrupt clear mask bit
#define INTC_MCR1_MSC2           0x00000008  // MSC1 interrupt clear mask bit
#define INTC_MCR1_UART4          0x00000004  // UART4 interrupt clear mask bit
#define INTC_MCR1_RTC            0x00000001  // RTC interrupt clear mask bit

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_PR1 register.
// 0: the corresponding interrupt is not active or is masked.
// 1: the corresponding interrupt is active or is not masked to the processor.
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_PR1_GPU            0x80000000  // GPU interrupt active/mask status
#define INTC_PR1_VPU            0x40000000  // VPU interrupt active/mask status
#define INTC_PR1_PDMAM          0x20000000  // PDMAM interrupt active/mask status
#define INTC_PR1_I2C0           0x10000000  // I2C0 interrupt active/mask status
#define INTC_PR1_I2C1           0x08000000  // I2C1 interrupt active/mask status
#define INTC_PR1_I2C2           0x04000000  // I2C2 interrupt active/mask status
#define INTC_PR1_I2C3           0x02000000  // I2C3 interrupt active/mask status
#define INTC_PR1_ETHC           0x00800000  // ETHC interrupt active/mask status
#define INTC_PR1_EFUSE          0x00400000  // EFUSE interrupt active/mask status
#define INTC_PR1_DDR            0x00100000  // DDR interrupt active/mask status
#define INTC_PR1_UART0          0x00080000  // UART0 interrupt active/mask status
#define INTC_PR1_UART1          0x00040000  // UART1 interrupt active/mask status
#define INTC_PR1_UART2          0x00020000  // UART2 interrupt active/mask status
#define INTC_PR1_UART3          0x00010000  // UART3 interrupt active/mask status
#define INTC_PR1_CPM            0x00008000  // CPM interrupt active/mask status
#define INTC_PR1_HARB0          0x00004000  // HARB0 interrupt active/mask status
#define INTC_PR1_HARB1          0x00002000  // HARB1 interrupt active/mask status
#define INTC_PR1_HARB2          0x00001000  // HARB2 interrupt active/mask status
#define INTC_PR1_PCM0           0x00000100  // PCM0 interrupt active/mask status
#define INTC_PR1_NFI            0x00000080  // NFI interrupt active/mask status
#define INTC_PR1_MSC0           0x00000020  // MSC0 interrupt active/mask status
#define INTC_PR1_MSC1           0x00000010  // MSC1 interrupt active/mask status
#define INTC_PR1_MSC2           0x00000008  // MSC1 interrupt active/mask status
#define INTC_PR1_UART4          0x00000004  // UART4 interrupt active/mask status
#define INTC_PR1_RTC            0x00000001  // RTC interrupt active/mask status

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DSR0 register.
// 0: not pending, 1: pending
//
//*****************************************************************************
#define INTC_DSR0_LCD            0x80000000  // LCD DMA interrupt pending
#define INTC_DSR0_DELAY_LINE     0x40000000  // DELAY_LINE DMA interrupt pending
#define INTC_DSR0_ISP            0x20000000  // ISP DMA interrupt pending
#define INTC_DSR0_TCU0           0x08000000  // TCU0 DMA interrupt pending
#define INTC_DSR0_TCU1           0x04000000  // TCU1 DMA interrupt pending
#define INTC_DSR0_TCU2           0x02000000  // TCU2 DMA interrupt pending
#define INTC_DSR0_AES            0x00800000  // AES DMA interrupt pending
#define INTC_DSR0_HASH           0x00400000  // HASH DMA interrupt pending
#define INTC_DSR0_OTG            0x00200000  // OTG DMA interrupt pending
#define INTC_DSR0_EHCI           0x00100000  // EHCI DMA interrupt pending
#define INTC_DSR0_EPDC           0x00080000  // EPDC DMA interrupt pending
#define INTC_DSR0_SADC           0x00040000  // SADC DMA interrupt pending
#define INTC_DSR0_GPIO0          0x00020000  // GPIO0 DMA interrupt pending
#define INTC_DSR0_GPIO1          0x00010000  // GPIO1 DMA interrupt pending
#define INTC_DSR0_GPIO2          0x00008000  // GPIO2 DMA interrupt pending
#define INTC_DSR0_GPIO3          0x00004000  // GPIO3 DMA interrupt pending
#define INTC_DSR0_GPIO4          0x00002000  // GPIO4 DMA interrupt pending
#define INTC_DSR0_GPIO5          0x00001000  // GPIO5 DMA interrupt pending
#define INTC_DSR0_PDMA           0x00000400  // PDMA DMA interrupt pending
#define INTC_DSR0_SSI0           0x00000100  // SSI0 DMA interrupt pending
#define INTC_DSR0_SSI1           0x00000080  // SSI1 DMA interrupt pending
#define INTC_DSR0_IPU            0x00000040  // IPU DMA interrupt pending
#define INTC_DSR0_OHCI           0x00000020  // OHCI DMA interrupt pending
#define INTC_DSR0_CSI            0x00000010  // CSI DMA interrupt pending
#define INTC_DSR0_DSI            0x00000008  // DSI DMA interrupt pending
#define INTC_DSR0_BCH            0x00000004  // BCH DMA interrupt pending
#define INTC_DSR0_AIC0           0x00000002  // AIC0 DMA interrupt pending
#define INTC_DSR0_DMIC           0x00000001  // DMIC DMA interrupt pending

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DMR0 register.
// 1: masked, 0: not masked
//
//*****************************************************************************
#define INTC_DMR0_LCD            0x80000000  // LCD DMA interrupt masked
#define INTC_DMR0_DELAY_LINE     0x40000000  // DELAY_LINE DMA interrupt masked
#define INTC_DMR0_ISP            0x20000000  // ISP DMA interrupt masked
#define INTC_DMR0_TCU0           0x08000000  // TCU0 DMA interrupt masked
#define INTC_DMR0_TCU1           0x04000000  // TCU1 DMA interrupt masked
#define INTC_DMR0_TCU2           0x02000000  // TCU2 DMA interrupt masked
#define INTC_DMR0_AES            0x00800000  // AES DMA interrupt masked
#define INTC_DMR0_HASH           0x00400000  // HASH DMA interrupt masked
#define INTC_DMR0_OTG            0x00200000  // OTG DMA interrupt masked
#define INTC_DMR0_EHCI           0x00100000  // EHCI DMA interrupt masked
#define INTC_DMR0_EPDC           0x00080000  // EPDC DMA interrupt masked
#define INTC_DMR0_SADC           0x00040000  // SADC DMA interrupt masked
#define INTC_DMR0_GPIO0          0x00020000  // GPIO0 DMA interrupt masked
#define INTC_DMR0_GPIO1          0x00010000  // GPIO1 DMA interrupt masked
#define INTC_DMR0_GPIO2          0x00008000  // GPIO2 DMA interrupt masked
#define INTC_DMR0_GPIO3          0x00004000  // GPIO3 DMA interrupt masked
#define INTC_DMR0_GPIO4          0x00002000  // GPIO4 DMA interrupt masked
#define INTC_DMR0_GPIO5          0x00001000  // GPIO5 DMA interrupt masked
#define INTC_DMR0_PDMA           0x00000400  // PDMA DMA interrupt masked
#define INTC_DMR0_SSI0           0x00000100  // SSI0 DMA interrupt masked
#define INTC_DMR0_SSI1           0x00000080  // SSI1 DMA interrupt masked
#define INTC_DMR0_IPU            0x00000040  // IPU DMA interrupt masked
#define INTC_DMR0_OHCI           0x00000020  // OHCI DMA interrupt masked
#define INTC_DMR0_CSI            0x00000010  // CSI DMA interrupt masked
#define INTC_DMR0_DSI            0x00000008  // DSI DMA interrupt masked
#define INTC_DMR0_BCH            0x00000004  // BCH DMA interrupt masked
#define INTC_DMR0_AIC0           0x00000002  // AIC0 DMA interrupt masked
#define INTC_DMR0_DMIC           0x00000001  // DMIC DMA interrupt masked

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DPR0 register.
// 0: the corresponding interrupt is not active or is masked.
// 1: the corresponding interrupt is active or is not masked to the processor.
//
//*****************************************************************************
#define INTC_DPR0_LCD            0x80000000  // LCD DMA interrupt active/mask status
#define INTC_DPR0_DELAY_LINE     0x40000000  // DELAY_LINE DMA interrupt active/mask status
#define INTC_DPR0_ISP            0x20000000  // ISP DMA interrupt active/mask status
#define INTC_DPR0_TCU0           0x08000000  // TCU0 DMA interrupt active/mask status
#define INTC_DPR0_TCU1           0x04000000  // TCU1 DMA interrupt active/mask status
#define INTC_DPR0_TCU2           0x02000000  // TCU2 DMA interrupt active/mask status
#define INTC_DPR0_AES            0x00800000  // AES DMA interrupt active/mask status
#define INTC_DPR0_HASH           0x00400000  // HASH DMA interrupt active/mask status
#define INTC_DPR0_OTG            0x00200000  // OTG DMA interrupt active/mask status
#define INTC_DPR0_EHCI           0x00100000  // EHCI DMA interrupt active/mask status
#define INTC_DPR0_EPDC           0x00080000  // EPDC DMA interrupt active/mask status
#define INTC_DPR0_SADC           0x00040000  // SADC DMA interrupt active/mask status
#define INTC_DPR0_GPIO0          0x00020000  // GPIO0 DMA interrupt active/mask status
#define INTC_DPR0_GPIO1          0x00010000  // GPIO1 DMA interrupt active/mask status
#define INTC_DPR0_GPIO2          0x00008000  // GPIO2 DMA interrupt active/mask status
#define INTC_DPR0_GPIO3          0x00004000  // GPIO3 DMA interrupt active/mask status
#define INTC_DPR0_GPIO4          0x00002000  // GPIO4 DMA interrupt active/mask status
#define INTC_DPR0_GPIO5          0x00001000  // GPIO5 DMA interrupt active/mask status
#define INTC_DPR0_PDMA           0x00000400  // PDMA DMA interrupt active/mask status
#define INTC_DPR0_SSI0           0x00000100  // SSI0 DMA interrupt active/mask status
#define INTC_DPR0_SSI1           0x00000080  // SSI1 DMA interrupt active/mask status
#define INTC_DPR0_IPU            0x00000040  // IPU DMA interrupt active/mask status
#define INTC_DPR0_OHCI           0x00000020  // OHCI DMA interrupt active/mask status
#define INTC_DPR0_CSI            0x00000010  // CSI DMA interrupt active/mask status
#define INTC_DPR0_DSI            0x00000008  // DSI DMA interrupt active/mask status
#define INTC_DPR0_BCH            0x00000004  // BCH DMA interrupt active/mask status
#define INTC_DPR0_AIC0           0x00000002  // AIC0 DMA interrupt active/mask status
#define INTC_DPR0_DMIC           0x00000001  // DMIC DMA interrupt active/mask status

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DSR1 register.
// 1: pending, 0: not pending
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_DSR1_GPU            0x80000000  // GPU DMA interrupt pending
#define INTC_DSR1_VPU            0x40000000  // VPU DMA interrupt pending
#define INTC_DSR1_PDMAM          0x20000000  // PDMAM DMA interrupt pending
#define INTC_DSR1_I2C0           0x10000000  // I2C0 DMA interrupt pending
#define INTC_DSR1_I2C1           0x08000000  // I2C1 DMA interrupt pending
#define INTC_DSR1_I2C2           0x04000000  // I2C2 DMA interrupt pending
#define INTC_DSR1_I2C3           0x02000000  // I2C3 DMA interrupt pending
#define INTC_DSR1_ETHC           0x00800000  // ETHC DMA interrupt pending
#define INTC_DSR1_EFUSE          0x00400000  // EFUSE DMA interrupt pending
#define INTC_DSR1_DDR            0x00100000  // DDR DMA interrupt pending
#define INTC_DSR1_UART0          0x00080000  // UART0 DMA interrupt pending
#define INTC_DSR1_UART1          0x00040000  // UART1 DMA interrupt pending
#define INTC_DSR1_UART2          0x00020000  // UART2 DMA interrupt pending
#define INTC_DSR1_UART3          0x00010000  // UART3 DMA interrupt pending
#define INTC_DSR1_CPM            0x00008000  // CPM DMA interrupt pending
#define INTC_DSR1_HARB0          0x00004000  // HARB0 DMA interrupt pending
#define INTC_DSR1_HARB1          0x00002000  // HARB1 DMA interrupt pending
#define INTC_DSR1_HARB2          0x00001000  // HARB2 DMA interrupt pending
#define INTC_DSR1_PCM0           0x00000100  // PCM0 DMA interrupt pending
#define INTC_DSR1_NFI            0x00000080  // NFI DMA interrupt pending
#define INTC_DSR1_MSC0           0x00000020  // MSC0 DMA interrupt pending
#define INTC_DSR1_MSC1           0x00000010  // MSC1 DMA interrupt pending
#define INTC_DSR1_MSC2           0x00000008  // MSC1 DMA interrupt pending
#define INTC_DSR1_UART4          0x00000004  // UART4 DMA interrupt pending
#define INTC_DSR1_RTC            0x00000001  // RTC DMA interrupt pending

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DMR1 register.
// 1: masked, 0: not masked
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_DMR1_GPU            0x80000000  // GPU DMA interrupt masked
#define INTC_DMR1_VPU            0x40000000  // VPU DMA interrupt masked
#define INTC_DMR1_PDMAM          0x20000000  // PDMAM DMA interrupt masked
#define INTC_DMR1_I2C0           0x10000000  // I2C0 DMA interrupt masked
#define INTC_DMR1_I2C1           0x08000000  // I2C1 DMA interrupt masked
#define INTC_DMR1_I2C2           0x04000000  // I2C2 DMA interrupt masked
#define INTC_DMR1_I2C3           0x02000000  // I2C3 DMA interrupt masked
#define INTC_DMR1_ETHC           0x00800000  // ETHC DMA interrupt masked
#define INTC_DMR1_EFUSE          0x00400000  // EFUSE DMA interrupt masked
#define INTC_DMR1_DDR            0x00100000  // DDR DMA interrupt masked
#define INTC_DMR1_UART0          0x00080000  // UART0 DMA interrupt masked
#define INTC_DMR1_UART1          0x00040000  // UART1 DMA interrupt masked
#define INTC_DMR1_UART2          0x00020000  // UART2 DMA interrupt masked
#define INTC_DMR1_UART3          0x00010000  // UART3 DMA interrupt masked
#define INTC_DMR1_CPM            0x00008000  // CPM DMA interrupt masked
#define INTC_DMR1_HARB0          0x00004000  // HARB0 DMA interrupt masked
#define INTC_DMR1_HARB1          0x00002000  // HARB1 DMA interrupt masked
#define INTC_DMR1_HARB2          0x00001000  // HARB2 DMA interrupt masked
#define INTC_DMR1_PCM0           0x00000100  // PCM0 DMA interrupt masked
#define INTC_DMR1_NFI            0x00000080  // NFI DMA interrupt masked
#define INTC_DMR1_MSC0           0x00000020  // MSC0 DMA interrupt masked
#define INTC_DMR1_MSC1           0x00000010  // MSC1 DMA interrupt masked
#define INTC_DMR1_MSC2           0x00000008  // MSC1 DMA interrupt masked
#define INTC_DMR1_UART4          0x00000004  // UART4 DMA interrupt masked
#define INTC_DMR1_RTC            0x00000001  // RTC DMA interrupt masked

//*****************************************************************************
//
// The following are defines for the bit fields in the INTC_O_DPR1 register.
// 0: the corresponding interrupt is not active or is masked.
// 1: the corresponding interrupt is active or is not masked to the processor.
// Note: ETHC is only for FPGA
//
//*****************************************************************************
#define INTC_DMR1_GPU            0x80000000  // GPU DMA interrupt active/mask status
#define INTC_DPR1_VPU            0x40000000  // VPU DMA interrupt active/mask status
#define INTC_DPR1_PDMAM          0x20000000  // PDMAM DMA interrupt active/mask status
#define INTC_DPR1_I2C0           0x10000000  // I2C0 DMA interrupt active/mask status
#define INTC_DPR1_I2C1           0x08000000  // I2C1 DMA interrupt active/mask status
#define INTC_DPR1_I2C2           0x04000000  // I2C2 DMA interrupt active/mask status
#define INTC_DPR1_I2C3           0x02000000  // I2C3 DMA interrupt active/mask status
#define INTC_DPR1_ETHC           0x00800000  // ETHC DMA interrupt active/mask status
#define INTC_DPR1_EFUSE          0x00400000  // EFUSE DMA interrupt active/mask status
#define INTC_DPR1_DDR            0x00100000  // DDR DMA interrupt active/mask status
#define INTC_DPR1_UART0          0x00080000  // UART0 DMA interrupt active/mask status
#define INTC_DPR1_UART1          0x00040000  // UART1 DMA interrupt active/mask status
#define INTC_DPR1_UART2          0x00020000  // UART2 DMA interrupt active/mask status
#define INTC_DPR1_UART3          0x00010000  // UART3 DMA interrupt active/mask status
#define INTC_DPR1_CPM            0x00008000  // CPM DMA interrupt active/mask status
#define INTC_DPR1_HARB0          0x00004000  // HARB0 DMA interrupt active/mask status
#define INTC_DPR1_HARB1          0x00002000  // HARB1 DMA interrupt active/mask status
#define INTC_DPR1_HARB2          0x00001000  // HARB2 DMA interrupt active/mask status
#define INTC_DPR1_PCM0           0x00000100  // PCM0 DMA interrupt active/mask status
#define INTC_DPR1_NFI            0x00000080  // NFI DMA interrupt active/mask status
#define INTC_DPR1_MSC0           0x00000020  // MSC0 DMA interrupt active/mask status
#define INTC_DPR1_MSC1           0x00000010  // MSC1 DMA interrupt active/mask status
#define INTC_DPR1_MSC2           0x00000008  // MSC1 DMA interrupt active/mask status
#define INTC_DPR1_UART4          0x00000004  // UART4 DMA interrupt active/mask status
#define INTC_DPR1_RTC            0x00000001  // RTC DMA interrupt active/mask status

#endif /* __HW_INTC_H__ */

