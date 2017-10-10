//*****************************************************************************
//
// msc.h - Defines and Macros for the MSC(MMC/SD Controller).
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

#ifndef __MSC_H__
#define __MSC_H__

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
// Struct for MSC inner DMA descriptor, used in DMA data transfer mode.
// SDMA is the special DMA in MSC controller
//
//*****************************************************************************
struct sdma_descriptor {
    volatile unsigned int NDA;  // Next descriptor physical address.
                                // The physical address of the next SDMA descriptor.
                                // It should be 4-word aligned
    volatile unsigned int DA;   // Data Address.
                                // The system memory address where the data will be read from or stored to.
    volatile unsigned int LEN;  // Unit is byte.
                                // The length of data to be transferred
    volatile unsigned int CMD;  // The command for the current SDMA.
                                // bit[31:24] Reserved It has no use in the current version.
                                // bit[23:16] ID Identification of Current DMA Transfer.
                                // bit[15:2] Reserved It has no use in the current version.
                                // bit[1] ENDI Interrupt Enable for Current DMA Transfer End.
                                //        0: Disable interrupt
                                //        1: Enable interrupt
                                // bit[0] LINK Control the end of DMA Descriptor
                                //        0: DMA will go to idle state after the current transfer.
                                //        1: DMA will continue to fetch another descriptor.
};

//*****************************************************************************
//
// Values that can be assign to sdma_descriptor.CMD
//
//*****************************************************************************
#define MSC_SDMACMD_ID          0x00FF0000  // bit[23:16] ID Identification of Current DMA Transfer
#define MSC_SDMACMD_ENDI        0x00000002  // bit[1] ENDI Interrupt Enable for Current DMA Transfer End
#define MSC_SDMACMD_LINK        0x00000001  // bit[0] LINK Control the end of DMA Descriptor

//*****************************************************************************
//
// Values that can be passed to MSCBusWidthSet as the ulBusWidth parameter
//
//*****************************************************************************
#define MSC_BUSWIDTH_1BIT       0x00000001  // 1 bit bus width
#define MSC_BUSWIDTH_4BIT       0x00000004  // 4 bit bus width
#define MSC_BUSWIDTH_8BIT       0x00000008  // 8 bit bus width

//*****************************************************************************
//
// Values that can be passed to MSCResponseFormatSet as the ulResponseFormat parameter
//
//*****************************************************************************
#define MSC_RESPONSE_NONE       0x00000000  // Non response format
#define MSC_RESPONSE_R1R1B      0x00000001  // Response Format R1 and R1b
#define MSC_RESPONSE_R2         0x00000002  // Response Format R2
#define MSC_RESPONSE_R3         0x00000003  // Response Format R3
#define MSC_RESPONSE_R4         0x00000004  // Response Format R4
#define MSC_RESPONSE_R5         0x00000005  // Response Format R5
#define MSC_RESPONSE_R6         0x00000006  // Response Format R6
#define MSC_RESPONSE_R7         0x00000007  // Response Format R7

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void MSCRegisterDump(unsigned long ulBase, int (*print)(const char *format, ...));
extern tBoolean MSCBusWidthSet(unsigned long ulBase, unsigned long ulBusWidth);
extern tBoolean MSCSDIOInterruptEnable(unsigned long ulBase);
extern tBoolean MSCSDIOInterruptDisable(unsigned long ulBase);
extern tBoolean MSCReadTimeOutSet(unsigned long ulBase, unsigned long ulTimeOutClks);
extern unsigned long MSCReadTimeOutGet(unsigned long ulBase);
extern tBoolean MSCBlockCountSet(unsigned long ulBase, unsigned long ulBlockCount);
extern unsigned long MSCBlockCountGet(unsigned long ulBase);
extern tBoolean MSCBlockSizeSet(unsigned long ulBase, unsigned long ulBlockSize);
extern unsigned long MSCBlockSizeGet(unsigned long ulBase);
extern tBoolean MSCStreamModeEnable(unsigned long ulBase);
extern tBoolean MSCStreamModeDisable(unsigned long ulBase);
extern tBoolean MSCResponseFormatSet(unsigned long ulBase, unsigned long ulResponseFormat);
extern tBoolean MSCBusySet(unsigned long ulBase);
extern tBoolean MSCBusyClear(unsigned long ulBase);
extern tBoolean MSCAutoCMD12Enable(unsigned long ulBase);
extern tBoolean MSCAutoCMD12Disable(unsigned long ulBase);
extern tBoolean MSCCommandIndexSet(unsigned long ulBase, unsigned long ulCommandIndex);
extern tBoolean MSCCommandArgumentSet(unsigned long ulBase, unsigned long ulCommandArgument);
extern tBoolean MSCNewOperationStart(unsigned long ulBase);
extern tBoolean MSCEndCommandResponseEnable(unsigned long ulBase);
extern tBoolean MSCEndCommandResponseDisable(unsigned long ulBase);
extern tBoolean MSCEndCommandResponseFlagClear(unsigned long ulBase);
extern tBoolean MSCEndCommandResponseFlagGet(unsigned long ulBase);
extern tBoolean MSCSDIOInterruptFlagGet(unsigned long ulBase);
extern unsigned long MSCCommandResponseGet(unsigned long ulBase);
extern tBoolean MSCDataTransferDoneEnable(unsigned long ulBase);
extern tBoolean MSCDataTransferDoneDisable(unsigned long ulBase);
extern tBoolean MSCDataTransferDoneFlagGet(unsigned long ulBase);
extern tBoolean MSCDataTransferDoneFlagClear(unsigned long ulBase);
extern tBoolean MSCDMADescriptorAddressSet(unsigned long ulBase, unsigned long ulDMADescriptorAddress);
extern unsigned long MSCDMADescriptorAddressGet(unsigned long ulBase);
extern tBoolean MSCDMADataDoneEnable(unsigned long ulBase);
extern tBoolean MSCDMADataDoneDisable(unsigned long ulBase);
extern tBoolean MSCDMADataDoneFlagGet(unsigned long ulBase);
extern tBoolean MSCDMADataDoneFlagClear(unsigned long ulBase);
extern tBoolean MSCDMAEndEnable(unsigned long ulBase);
extern tBoolean MSCDMAEndDisable(unsigned long ulBase);
extern tBoolean MSCDMAEndFlagGet(unsigned long ulBase);
extern tBoolean MSCDMAEndFlagClear(unsigned long ulBase);
extern tBoolean MSCDMAEnable(unsigned long ulBase);
extern tBoolean MSCDMADisable(unsigned long ulBase);
extern tBoolean MSCDataEnable(unsigned long ulBase);
extern tBoolean MSCDataDisable(unsigned long ulBase);
extern tBoolean MSCDataRead(unsigned long ulBase);
extern tBoolean MSCDataWrite(unsigned long ulBase);
extern tBoolean MSCRXFIFOReadRequestEnable(unsigned long ulBase);
extern tBoolean MSCRXFIFOReadRequestDisable(unsigned long ulBase);
extern tBoolean MSCRXFIFOReadRequestFlagGet(unsigned long ulBase);
extern tBoolean MSCRXFIFOReadRequestFlagClear(unsigned long ulBase);
extern tBoolean MSCDataFIFOFullEnable(unsigned long ulBase);
extern tBoolean MSCDataFIFOFullDisable(unsigned long ulBase);
extern tBoolean MSCDataFIFOFullFlagGet(unsigned long ulBase);
extern tBoolean MSCAutoCMD12DoneEnable(unsigned long ulBase);
extern tBoolean MSCAutoCMD12DoneDisable(unsigned long ulBase);
extern tBoolean MSCAutoCMD12DoneFlagGet(unsigned long ulBase);
extern tBoolean MSCAutoCMD12DoneFlagClear(unsigned long ulBase);
extern tBoolean MSCDataFIFOEmptyEnable(unsigned long ulBase);
extern tBoolean MSCDataFIFOEmptyDisable(unsigned long ulBase);
extern tBoolean MSCDataFIFOEmptyFlagGet(unsigned long ulBase);
extern tBoolean MSCCRCResponseErrorEnable(unsigned long ulBase);
extern tBoolean MSCCRCResponseErrorDisable(unsigned long ulBase);
extern tBoolean MSCCRCResponseErrorFlagGet(unsigned long ulBase);
extern tBoolean MSCCRCResponseErrorFlagClear(unsigned long ulBase);
extern tBoolean MSCCRCReadErrorEnable(unsigned long ulBase);
extern tBoolean MSCCRCReadErrorDisable(unsigned long ulBase);
extern tBoolean MSCCRCReadErrorFlagGet(unsigned long ulBase);
extern tBoolean MSCCRCReadErrorFlagClear(unsigned long ulBase);
extern tBoolean MSCLowPowerModeSet(unsigned long ulBase);
extern unsigned long MSCRXFIFOGet(unsigned long ulBase);
extern unsigned long MSCRTFIFOCountGet(unsigned long ulBase);
extern unsigned long MSCInterruptGet(unsigned long ulBase);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* __MSC_H__ */

