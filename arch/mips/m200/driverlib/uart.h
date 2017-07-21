//*****************************************************************************
//
// uart.h - Defines and Macros for the UART.
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

#ifndef __UART_H__
#define __UART_H__

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
// Values that can be passed to UARTConfigSetExpClk as the ulConfig parameter
// and returned by UARTConfigGetExpClk in the pulConfig parameter.
// Additionally, the UART_CONFIG_PAR_* subset can be passed to
// UARTParityModeSet as the ulParity parameter, and are returned by
// UARTParityModeGet.
//
//*****************************************************************************
#define UART_CONFIG_WLEN_MASK   0x00000003  // Mask for extracting word length
#define UART_CONFIG_WLEN_8      0x00000003  // 8 bit data
#define UART_CONFIG_WLEN_7      0x00000002  // 7 bit data
#define UART_CONFIG_WLEN_6      0x00000001  // 6 bit data
#define UART_CONFIG_WLEN_5      0x00000000  // 5 bit data
#define UART_CONFIG_STOP_MASK   0x00000004  // Mask for extracting stop bits
#define UART_CONFIG_STOP_ONE    0x00000000  // One stop bit
#define UART_CONFIG_STOP_TWO    0x00000004  // Two stop bits
#define UART_CONFIG_PAR_MASK    0x00000038  // Mask for extracting parity
#define UART_CONFIG_PAR_NONE    0x00000000  // No parity
#define UART_CONFIG_PAR_EVEN    0x00000018  // Even parity
#define UART_CONFIG_PAR_ODD     0x00000008  // Odd parity
#define UART_CONFIG_PAR_STICKY  0x00000020  // Sticky Parity(with Even/Odd)

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ulTxLevel parameter and
// returned by UARTFIFOLevelGet in the pulTxLevel.
//
//*****************************************************************************
#define UART_FIFO_SIZE          0x00000040  // 64x8bits transmit FIFO
                                            // 64x11bits receive FIFO
#define UART_FIFO_TX1_8         0x00000000  // Transmit interrupt at 1 of 64
#define UART_FIFO_TX2_8         0x00000001  // Transmit interrupt at 16 of 64
#define UART_FIFO_TX3_8         0x00000002  // Transmit interrupt at 32 of 64
#define UART_FIFO_TX4_8         0x00000003  // Transmit interrupt at 60 of 64

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ulRxLevel parameter and
// returned by UARTFIFOLevelGet in the pulRxLevel.
//
//*****************************************************************************
#define UART_FIFO_RX_MASK       0x000000C0  // Receive interrupt mask
#define UART_FIFO_RX1_64        0x00000000  // Receive interrupt at 1 of 64
#define UART_FIFO_RX16_64       0x00000040  // Receive interrupt at 16 of 64
#define UART_FIFO_RX32_64       0x00000080  // Receive interrupt at 32 of 64
#define UART_FIFO_RX60_64       0x000000C0  // Receive interrupt at 60 of 64

//*****************************************************************************
//
// Values returned from UARTRxErrorGet().
//
//*****************************************************************************
#define UART_RXERROR_MASK       0x0000009E
#define UART_RXERROR_FIFO       0x00000080
#define UART_RXERROR_BREAK      0x00000010
#define UART_RXERROR_FRAMIMG    0x00000008
#define UART_RXERROR_PARITY     0x00000004
#define UART_RXERROR_OVERRUN    0x00000002

//*****************************************************************************
//
// Values that can be passed to UARTModemControlSet()
//
//*****************************************************************************
#define UART_MODEM_LOOPBACK    0x00000010
#define UART_MODEM_RTS         0x00000002

//*****************************************************************************
//
// Values that can be passed to UARTModeSet() 
//
//*****************************************************************************
#define UART_MODE_MASK          0x00000003
#define UART_MODE_UART          0x00000000
#define UART_MODE_SIR           0x00000003

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern long UARTIntNumberGet(unsigned long ulBase);
extern tBoolean UARTTxEmpty(unsigned long ulBase);
extern tBoolean UARTRxStart(unsigned long ulBase);
extern tBoolean UARTRxStop(unsigned long ulBase);
extern tBoolean UARTTxStart(unsigned long ulBase);
extern tBoolean UARTTxStop(unsigned long ulBase);
extern void UARTEnable(unsigned long ulBase);
extern void UARTDisable(unsigned long ulBase);
extern void UARTRegisterDump(unsigned long ulBase,
		int (*print)(const char *format, ...));
extern void UARTModemControlSet(unsigned long ulBase, unsigned long ulControl);
extern unsigned long UARTModemControlGet(unsigned long ulBase);
extern tBoolean UARTModemStatusInterruptEnable(unsigned long ulBase);
extern tBoolean UARTCharPutNonBlocking(unsigned long ulBase,
		unsigned char ucData);
extern void UARTCharPut(unsigned long ulBase, unsigned char ucData);
extern unsigned char UARTCharGet(unsigned long ulBase);
extern void UARTFIFOEnable(unsigned long ulBase);
extern void UARTFIFODisable(unsigned long ulBase);
extern void UARTFIFOLevelSet(unsigned long ulBase, unsigned long ulRxLevel);
extern tBoolean UARTIsTxInterrupt(unsigned long ulBase, unsigned long ulLsr);
extern tBoolean UARTIsRxInterrupt(unsigned long ulBase, unsigned long ulLsr);
extern tBoolean UARTIsRxBreakInterrupt(unsigned long ulBase,
		unsigned long ulLsr);
extern unsigned long UARTLsrGet(unsigned long ulBase);
extern tBoolean UARTLsrClear(unsigned long ulBase);
extern void UARTParityModeSet(unsigned long ulBase, unsigned long ulParity);
extern void UARTConfigSetExpClk(unsigned long ulBase, unsigned long ulUARTClk,
                    unsigned long ulBaud, unsigned long ulConfig);
extern unsigned long UARTRxErrorGet(unsigned long ulBase);
extern void UARTModeSet(unsigned long ulBase, unsigned long ulMode);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */

