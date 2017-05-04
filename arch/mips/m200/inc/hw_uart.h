//*****************************************************************************
//
// hw_uart.h - Macros used when accessing the UART hardware.
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

#ifndef __HW_UART_H__
#define __HW_UART_H__

//*****************************************************************************
//
// The following are defines for the UART register offsets.
//
//*****************************************************************************
#define UART_O_RBR              0x00000000  // UART Receive Buffer Register
#define UART_O_THR              0x00000000  // UART Transmit Hold Register
#define UART_O_DLLR             0x00000000  // UART Divisor Latch Low Register
#define UART_O_DLHR             0x00000004  // UART Divisor Latch High Register
#define UART_O_IER              0x00000004  // UART Interrupt Enable Register
#define UART_O_IIR              0x00000008  // UART Interrupt Identification Register
#define UART_O_FCR              0x00000008  // UART FIFO Control Register
#define UART_O_LCR              0x0000000C  // UART Line Control Register
#define UART_O_MCR              0x00000010  // UART Modem Control Register
#define UART_O_LSR              0x00000014  // UART Line Status Register
#define UART_O_MSR              0x00000018  // UART Modem Status Register
#define UART_O_SPR              0x0000001C  // UART Scratchpad Register
#define UART_O_ISR              0x00000020  // Infrared Selection Register
#define UART_O_MR               0x00000024  // UART M Register
#define UART_O_ACR              0x00000028  // UART Add Cycle Register
#define UART_O_RFCR             0x00000040  // UART RXFIFO Counter Register
#define UART_O_TFCR             0x00000044  // UART TXFIFO Counter Register

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_RBR register.
//
//*****************************************************************************
#define UART_RBR_URBR           0x000000FF

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_THR register.
//
//*****************************************************************************
#define UART_THR_UTHR           0x000000FF

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_DLLR register.
//
//*****************************************************************************
#define UART_DLLR_DLL           0x000000FF

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_DLHR register.
//
//*****************************************************************************
#define UART_DLHR_DLH           0x000000FF

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_IER register.
//
//*****************************************************************************
#define UART_IER_RTOIE          0x00000010 // Receive Timeout Interrupt Enable
#define UART_IER_MSIE           0x00000008 // Modem Status Interrupt Enable
#define UART_IER_RLSIE          0x00000004 // Receive Line Status Interrupt Enable
#define UART_IER_TDRIE          0x00000002 // Transmit Data Request Interrupt Enable
#define UART_IER_RDRIE          0x00000001 // Receive Data Ready Interrupt Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_IIR register.
//
//*****************************************************************************
#define UART_IIR_FFMSEL         0x000000C0 // FIFO Mode Select
#define UART_IIR_INID           0x0000000E // Interrupt Identifier
#define UART_IIR_INPEND         0x00000001 // Interrupt Pending

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_FCR register.
//
//*****************************************************************************
#define UART_FCR_RDTR           0x000000C0 // Receive Buffer Data Number Trigger
#define UART_FCR_UME            0x00000010 // UART Module Enable
#define UART_FCR_DME            0x00000008 // DMA Mode Enable
#define UART_FCR_TFRT           0x00000004 // Transmit Holding Register Reset
#define UART_FCR_RFRT           0x00000002 // Receive Buffer Reset
#define UART_FCR_FME            0x00000001 // FIFO Mode Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LCR register.
//
//*****************************************************************************
#define UART_LCR_DLAB           0x00000080 // Divisor Latch Access Bit
#define UART_LCR_SBK            0x00000040 // Set Break
#define UART_LCR_STPAR          0x00000020 // Sticky Parity
#define UART_LCR_PARM           0x00000010 // Parity Odd/Even Mode Select
#define UART_LCR_PARE           0x00000008 // Parity Enable
#define UART_LCR_SBLS           0x00000004 // Stop Bit Length Select
#define UART_LCR_WLS            0x00000003 // Word Length Select

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_MCR register.
//
//*****************************************************************************
#define UART_MCR_MDCE           0x00000080 // Modem Control Enable
#define UART_MCR_FCM            0x00000040 // Flow Control Mode
#define UART_MCR_LOOP           0x00000010 // Loop Back
#define UART_MCR_RTS            0x00000010 // Request To Send

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LSR register.
//
//*****************************************************************************
#define UART_LSR_FIFOE          0x00000080 // FIFO Error Status. (FIFO mode only)
#define UART_LSR_TEMP           0x00000040 // Transmit Holding Register Empty
#define UART_LSR_TDRQ           0x00000020 // Transmit Data Request
#define UART_LSR_BI             0x00000010 // Break Interrupt
#define UART_LSR_FMER           0x00000008 // Framing Error
#define UART_LSR_PARER          0x00000004 // Parity Error
#define UART_LSR_OVER           0x00000002 // Overrun Error
#define UART_LSR_DRY            0x00000001 // Data Ready

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_MSR register.
//
//*****************************************************************************
#define UART_MSR_CTS            0x00000010 // Status of Clear To Send
#define UART_MSR_CCTS           0x00000001 // Change status of CTS_ pin

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_SPR register.
//
//*****************************************************************************
#define UART_SPR_SCRATCH        0x000000FF // no effect on the UART

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_ISR register.
//
//*****************************************************************************
#define UART_ISR_RDPL           0x00000010 // Receive Data Polarity
#define UART_ISR_TDPL           0x00000008 // Transmit Data Polarity
#define UART_ISR_XMODE          0x00000004 // Transmit Pulse Width Mode
#define UART_ISR_RCVEIR         0x00000002 // Receiver SIR Enable
#define UART_ISR_XMITIR         0x00000001 // Transmitter SIR Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_MR register.
//
//*****************************************************************************
#define UART_MR_M               0x0000001F // [M, M+1] cycles to transmit or 
                                           // receive one bit

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_ACR register.
//
//*****************************************************************************
#define UART_ACR_AC             0x00000FFF // [AC] == 0, add zero cycle
                                           // [AC] = (0, 0xFFF) insert cycle
                                           // [AC] == 0xFFF, add one cycle

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_RFCR register.
//
//*****************************************************************************
#define UART_RFCR_RCNT          0x0000007F // RXFIFO Counter

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_TFCR register.
//
//*****************************************************************************
#define UART_TFCR_TCNT          0x0000007F // TXFIFO Counter

#endif /* __HW_UART_H__ */

