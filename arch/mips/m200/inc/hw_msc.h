//*****************************************************************************
//
// hw_mmc.h - Macros used when accessing the MMC/SD Controller(MSC) hardware.
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

#ifndef __HW_MSC_H__
#define __HW_MSC_H__

//*****************************************************************************
//
// The following are defines for the MMC register offsets.
//
//*****************************************************************************
#define MSC_O_CTRL              0x00000000  // MSC Control Register 16-bits
#define MSC_O_STAT              0x00000004  // MSC Status Register
#define MSC_O_CLKRT             0x00000008  // MSC Clock Rate Register 16-bits
#define MSC_O_CMDAT             0x0000000C  // MSC Command and Data Control Register
#define MSC_O_RESTO             0x00000010  // MSC Response Time Out Register 16-bits
#define MSC_O_RDTO              0x00000014  // MSC Read Time Out Register
#define MSC_O_BLKEN             0x00000018  // MSC Block Size Register 16-bits
#define MSC_O_NOB               0x0000001C  // MSC Block Counter Register 16-bits
#define MSC_O_SNOB              0x00000020  // MSC Successfull-transfered Blocks Count Register
#define MSC_O_IMASK             0x00000024  // MSC Interrupt Mask Register
#define MSC_O_IFLG              0x00000028  // MSC Interrupt Flag Register
#define MSC_O_CMD               0x0000002C  // MSC Command Index Register 8-bits
#define MSC_O_ARG               0x00000030  // MSC Command Argument Register
#define MSC_O_RES               0x00000034  // MSC Response FIFO Register 16-bits
#define MSC_O_RXFIFO            0x00000038  // MSC Receive FIFO Port Register
#define MSC_O_TXFIFO            0x0000003C  // MSC Transmit FIFO Port Register
#define MSC_O_LPM               0x00000040  // MSC Low Power Mode Register
#define MSC_O_DMAC              0x00000044  // MSC DMA Control Register
#define MSC_O_DMANDA            0x00000048  // MSC DMA Descriptor Address Register
#define MSC_O_DMADA             0x0000004C  // MSC DMA Data Address Register
#define MSC_O_DMALEN            0x00000050  // MSC DMA Data Length Register
#define MSC_O_DMACMD            0x00000054  // MSC DMA Command Register
#define MSC_O_CTRL2             0x00000058  // MSC Control Register 2
#define MSC_O_RTCNT             0x0000005C  // MSC RTFIFO Data Counter Register

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_CTRL register.
//
//*****************************************************************************
#define MSC_CTRL_SENDCCSD       0x00008000  // 0: Clear bit
                                            // 1: Send Command Completion Signal Disable(CCSD) to CE_ATA device
#define MSC_CTRL_SENDASCCSD     0x00004000  // 0: Clear bit
                                            // 1: Send internally generated stop after sending CCSD to CE_ATA device
#define MSC_CTRL_EXITMULTIPLE   0x00000080  // 0: No effect
                                            // 1: Exit from multiple block read/write
                                            // If CMD12 or CMD52(I/O abort) is to be send to terminate multiple blovk read/write in advance, set this bit to 1.
#define MSC_CTRL_EXITTRANSFER   0x00000040  // 0: No effect
                                            // 1: Exit from multiple block read/write after suspend is accepted, or exit from stream read
                                            // Only used for SDIO suspend/resume and MMC stream read.
                                            // For SDIO, after suspend is accepted, set this bit with 1.
                                            // For MMC, after the data of the expected number are received, set this bit with 1.
#define MSC_CTRL_STARTREADWAIT  0x00000020  // 0: No effect
                                            // 1: Start Read Wait. Only used for SDIO Read Wait. Start the Read Wait cycle.
#define MSC_CTRL_STOPREADWAIT   0x00000010  // Only used for SDIO Read Wait. Stop the Read Wait cycle.
                                            // 0: No effect
                                            // 1: Stop Read Wait
#define MSC_CTRL_RESET          0x00000008  // 0: No effect
                                            // 1: Rest the MSC Controller
#define MSC_CTRL_STARTOP        0x00000004  // 0: Do nothing
                                            // 1: Start the new operation
                                            // When starting the clock, this bit can be 1. When stopping the clock, this bit can be 0.
#define MSC_CTRL_CLOCKCTRL      0x00000003  // 00: Do nothing
                                            // 01: Stop MMC/SD clock
                                            // 10: Start MMC/SD clock
                                            // 11: Reserved

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_STAT register.
//
//*****************************************************************************
#define MSC_STAT_AUTOCMD12DONE  0x80000000  // Indicate that the stop command(CMD12) that is internally generated by controller has finished.
#define MSC_STAT_PINLEVEL       0x1F000000  // MSC interface Pin Level
                                            // PINLEVEL[4:0] indicates the pin level of MSC_CMD, MSC_DAT[3:0] separately.
#define MSC_STAT_BCE            0x00100000  // Boot CRC error
                                            // 0: No boot CRC error occurs
                                            // 1: Boot CRC is not correct
#define MSC_STAT_BDE            0x00080000  // Boot data end
                                            // 0: No boot data or boot data is not finished.
                                            // 1: Boot data is received completely.
#define MSC_STAT_BAE            0x00040000  // Boot acknowledge is error.
                                            // 0: No boot acknowledge is received or boot acknowledge is correct(decided by BAR)
                                            // 1: The received boot acknowledge is not correct.
#define MSC_STAT_BAR            0x00020000  // Boot acknowledge received
                                            // 0: No boot acknowledge received.
                                            // 1: Boot acknowledge received.
#define MSC_STAT_DMAEND         0x00010000  // Indicates that the DMA has finished the current transfer
#define MSC_STAT_ISRESETTING    0x00008000  // 0: Reset has been finished
                                            // 1: Reset has not been finished.
#define MSC_STAT_SDIOINTACTIVE  0x00004000  // Indicates wheather an interrut is detected at the SD I/O card. A seperate acknowledge command to card is required to clear this interrupt.
                                            // 0: No interrupt detected.
                                            // 1: The interrupt from SDIO is detected.
#define MSC_STAT_PRGDONE        0x00002000  // Indicates whether card has finished programming
                                            // 0: Not finished. 1: Finished.
#define MSC_STAT_DATATRANDONE   0x00001000  // Indicates whether data transmission to card has completed.
                                            // 0: Not completed. 1: Completed.
#define MSC_STAT_ENDCMDRES      0x00000800  // Indicates whether command and response/no-response sequence have been completed.
                                            // 0: Not completed. 1: Completed.
#define MSC_STAT_DATAFIFOAFULL  0x00000400  // Indicate whether the data FIFO is almost full.
                                            // 0: The number of words in FIFO is less than 127.
                                            // 1: The number of words in FIFO is equal to or greater than 127.
#define MSC_STAT_ISREADWAIT     0x00000200  // Indicates whether SDIO card has enterred Read Wait State.
                                            // 0: Card has not entered Read Wait State
                                            // 1: Card has entered Read Wait State
#define MSC_STAT_CLKEN          0x00000100  // Indicates whether clock is enabled
                                            // 0: Clock is off. 1: Clock has been enabled.
#define MSC_STAT_DATAFIFOFULL   0x00000080  // Indicates whether the data FIFO is full.
                                            // 0: Data FIFO is not full. 1: Data FIFO is full.
#define MSC_STAT_DATAFIFOEMPTY  0x00000040  // Indicates whether data FIFO is empty.
                                            // 0: Data FIFO is not empty. 1: Data FIFO is empty.
#define MSC_STAT_CRCRESERR      0x00000020  // Indicates Response CRC error
                                            // 0: No error on the response CRC
                                            // 1: CRC error occured on the response.
#define MSC_STAT_CRCREADERROR   0x00000010  // Indicates Read CRC error
                                            // 0: No error on received data.
                                            // 1: CRC error occurred on received data.
#define MSC_STAT_CRCWRITEERROR  0x0000000C  // Indicates Write CRC error
                                            // 00: No error on transmission of data
                                            // 01: Card observed erroneous transmission data.
                                            // 10: No CRC status is send back.
                                            // 11: Reserved.
#define MSC_STAT_TIMEOUTRES     0x00000002  // Response Time Out
                                            // 0: Card response has not timed out
                                            // 1: Card response has time out.
#define MSC_STAT_TIMEOUTREAD    0x00000001  // Read time out
                                            // 0: Card read data has not timed out.
                                            // 1: Card read data has timed out.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_CLKRT register.
// Note: This field must be set to 0 when the controller works during normal writing or reading.
//
//*****************************************************************************
#define MSC_CLKRT_CLKRATE       0x00000007  // Clock rate
                                            // 000: DEV_CLK
                                            // 001: 1/2 DEV_CLK
                                            // 010: 1/4 DEV_CLK
                                            // 011: 1/8 DEV_CLK
                                            // 100: 1/16 DEV_CLK
                                            // 101: 1/32 DEV_CLK
                                            // 110: 1/64 DEV_CLK
                                            // 111: 1/128 DEV_CLK

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_CMDAT register.
//
//*****************************************************************************
#define MSC_CMDAT_CCSEXPECTED   0x80000000  // 0: Interrupts are not enabled in CE-ATA device, or commands does not expect CCS from device.
                                            // 1: Interrups are enabled in CE-ATA device, or RW_BLK command expects command completion signal(CCS) from device
                                            // If the command expects CCS from device, the software should set the control bit. It is auto cleared 0 by hardware.
#define MSC_CMDAT_READCEATA     0x40000000  // 0: Host is not performing read access (RW_BLK or RW_REG) towards CE-ATA device.
                                            // 1: Host is performing read access(RW_BLK or RW_reg) towards CE-ATA device.
                                            // Software should set the bit to indicate the CE-ATA device is being accessed for read transfer. The bit is used to disable read data timeout indication while performing CE_ATA read trnasfer.
                                            // It is auto cleared 0 by hardware.
#define MSC_CMDAT_DISBOOT       0x08000000  // 0: Do nothing
                                            // 1: Stop boot operation
                                            // DIS_BOOT should not be used with ENA_BOOT at the same time.
#define MSC_CMDAT_EXPBOOTACK    0x02000000  // Whether boot acknowledge pattern is expected or not
                                            // 0: Boot acknowledge pattern is not expected.
                                            // 1: Boot acknowledge pattern is expected.
#define MSC_CMDAT_BOOTMODE      0x01000000  // Boot mode operation selection.
                                            // 0: Ignored
                                            // 1: Alternative boot operation
#define MSC_CMDAT_SDIOPRDT      0x00020000  // Determine whether SDIO interrupt is 2 cycle or extend more cycle when data block last is transfered.
                                            // 0: More cycle(like single block)
                                            // 1: Exact 2 cycle
#define MSC_CMDAT_AUTOCMD12     0x00010000  // This field controls use of auto command functions.
                                            // 0: Auto CMD12 is disabled.
                                            // 1: MSC controller will issue CMD12 automatically after the last data block transfer is finished.
                                            // When stop command has finished, it is auto cleared 0 by hardware.
#define MSC_CMDAT_RTRG          0x0000C000  // Receive FIFO Trigger Value Select.
                                            // Thes bits set the receive FIFO half-empty threshold value, when the number of transmit FIFO >= threshold value, RXFIFO_RD_REQ will be set to 1.
                                            // 00: More then or equal to 16
                                            // 01: More then or equal to 32
                                            // 10: More then or equal to 64
                                            // 11: More then or equal to 96
#define MSC_CMDAT_TTRG          0x00003000  // Transmit FIFO Trigger Value Select.
                                            // Thes bits set the transmit FIFO half-empty threshold value, when the number of transmit FIFO < threshold value, TXFIFO_WR_REQ will be set to 1.
                                            // 00: Less than 16
                                            // 01: Less than 32
                                            // 10: Less than 64
                                            // 11: Less than 96
#define MSC_CMDAT_IOABORT       0x00000800  // Specifies the current command is used to abort data transfer.
                                            // 0: Nothing
                                            // 1: The current command is used to abort transfer.
                                            // It is auto cleared 0 by hardware.
#define MSC_CMDAT_BUSWIDTH      0x00000600  // Specifies the width of the data bus.
                                            // 00: 1-bit
                                            // 01: Reserved
                                            // 10: 4-bits
                                            // 11: 8-bits
#define MSC_CMDAT_BUSWIDTH_1    0x00000000
#define MSC_CMDAT_BUSWIDTH_4    0x00000200
#define MSC_CMDAT_BUSWIDTH_8    0x00000600

#define MSC_CMDAT_INIT          0x00000080  // 80 initialization clocks.
                                            // 0: Issues command directly.
                                            // 1: Issues 80 clocks before command.
#define MSC_CMDAT_BUSY          0x00000040  // Specifies whether a busy signal is expected after current command. This bit is for no data command/response transactions only.
                                            // 0: No expect a busy signal
                                            // 1: Expects a busy signal. If the response is R1b, then set it.
#define MSC_CMDAT_STREAMBLOCK   0x00000020  // Stream mode.
                                            // 0: Data transfer of the current command sequence is not in stream mode.
                                            // 1: Data transfer of the current command sequence is in stream mode.
#define MSC_CMDAT_WRITEREAD     0x00000010  // Data Transfer Direction Selection.
                                            // 0: Read(from card to host)
                                            // 1: Write(from host to card)
#define MSC_CMDAT_DATALEN       0x00000008  // Specifies whether the current command includes data transfer or not.
                                            // It is also used to reset RX_FIFO and TX_FIFO
                                            // 0: Current command without data transfer
                                            // 1: Current command with data transfer
#define MSC_CMDAT_RESPONSEFORMAT 0x00000007 // Response Type Selection
                                            // 000: No response
                                            // 001: Format R1 and R1b
                                            // 010: Format R2
                                            // 011: Format R3
                                            // 100: Format R4
                                            // 101: Format R5
                                            // 110: Format R6
                                            // 111: Format R7

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_RESTO register.
//
//*****************************************************************************
#define MSC_RESTO_RESTO         0x0000FFFF  // Specifies the maximum number of MSC_CLK clock cycles between the end bit of the command and the response from the SD card.
                                            // The default value is 128.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_RDTO register.
//
//*****************************************************************************
#define MSC_RDTO_READTO         0xFFFFFFFF  // Specifies the maximum number clocks between the command and when the MMC/SD host controller turns on the time-out error for the received data. The unit is MSC_CLK.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_BLKEN register.
//
//*****************************************************************************
#define MSC_BLKEN_BLKEN         0x0000FFFF  // Specifies the number of bytes in a block, and is normally set to 0x200 for MMC/SD data transactions. The value specified in the cards CSD.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_NOB register.
//
//*****************************************************************************
#define MSC_NOB_NOB             0x0000FFFF  // Specifies the number of blocks in a data transfer. One block is possibility.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_SNOB register.
//
//*****************************************************************************
#define MSC_SNOB_SNOB           0x0000FFFF  // Specify the number of successfully transferred block for a multiple block transfer.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_IMASK register.
//
//*****************************************************************************
#define MSC_IMASK_DMADATADONE   0x80000000  // Mask the interrupt DMA_DATA_DONE
                                            // 0: Not masked
                                            // 1: Masked
#define MSC_IMASK_RESERVED1     0x60000000  // Inner usage, keep to 1
#define MSC_IMASK_PINLEVEL      0x1F000000  // Mask the interrupt of PIN_LEVEL separately
#define MSC_IMASK_WRALLDONE     0x00800000  // Mask the interrupt WR_ALL_DONE
#define MSC_IMASK_BCE           0x00100000  // Boot CRC error
#define MSC_IMASK_BDE           0x00080000  // Boot data end
#define MSC_IMASK_BAE           0x00040000  // Mask the interrupt of BAE
#define MSC_IMASK_BAR           0x00020000  // Mask the interrupt of BAR
#define MSC_IMASK_DMAEND        0x00010000  // Mask the interrupt of DMA end
#define MSC_IMASK_AUTOCMD12DONE 0x00008000  // Mask the interrupt AUTO_CMD12_DONE
#define MSC_IMASK_DATAFIFOFULL  0x00004000  // Mask the interrupt DATA_FIFO_FULL
#define MSC_IMASK_DATAFIFOEMP   0x00002000  // Mask the interrupt DATA_FIFO_EMP
#define MSC_IMASK_CRCRESERR     0x00001000  // Mask the interrupt CRC_RES_ERR
#define MSC_IMASK_CRCREADERR    0x00000800  // Mask the interrupt CRC_READ_ERR
#define MSC_IMASK_CRCWRITEERR   0x00000400  // Mask the interrupt CRC_WRITE_ERR
#define MSC_IMASK_TIMEOUTRES    0x00000200  // Mask the interrupt TIME_OUT_RES
#define MSC_IMASK_TIMEOUTREAD   0x00000100  // Mask the interrupt TIME_OUT_READ
#define MSC_IMASK_SDIO          0x00000080  // Mask the interrupt from the SD I/O card
#define MSC_IMASK_TXFIFOWRREQ   0x00000040  // Mask the Transmit FIFO write request interrupt
#define MSC_IMASK_RXFIFORDREQ   0x00000020  // Mask the Receive FIFO read request interrupt
#define MSC_IMASK_ENDCMDRES     0x00000004  // Mask the End command response interrupt
#define MSC_IMASK_PRGDONE       0x00000002  // Mask the Programming done interrupt
#define MSC_IMASK_DATATRANDONE  0x00000001  // Mask the Data transfer done interrupt

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_IFLG register.
//
//*****************************************************************************
#define MSC_IFLG_DMADATADONE    0x80000000  // This field will be set to 1 when DATA_TRAN_DONE and DMAEND are valid.
                                            // 0: the interrupt is not detected
                                            // 1: the interrupt is detected
#define MSC_IFLG_RESERVED0      0x60000000  // Inner usage, keep to 0
#define MSC_IFLG_PINLEVEL       0x1F000000  // Indicates PIN_LEVEL interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_WRALLDONE      0x00800000  // This field will be set to 1 when DATA_TRAN_DONE, DMAEND and PRG_DONE are valid.
#define MSC_IFLG_BCE            0x00100000  // Boot CRC error
                                            // Write 1 to clear.
#define MSC_IFLG_BDE            0x00080000  // Boot data end
                                            // Write 1 to clear.
#define MSC_IFLG_BAE            0x00040000  // Indicates the BAE interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_BAR            0x00020000  // Indicates the BAR interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_DMAEND         0x00010000  // Indicates the DMA end interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_AUTOCMD12DONE  0x00008000  // Indicates AUTO_CMD12_DONE interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_DATAFIFOFULL   0x00004000  // Indicates data FIFO is full interrupt
#define MSC_IFLG_DATAFIFOEMP    0x00002000  // Indicates data FIFO is empty interrupt
#define MSC_IFLG_CRCRESERR      0x00001000  // Indicates response CRC error interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_CRCREADERR     0x00000800  // Indicates CRC read error interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_CRCWRITEERR    0x00000400  // Indicate CRC write error interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_TIMEOUTRES     0x00000200  // Indicate response time out interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_TIMEOUTREAD    0x00000100  // Indicate read time out interrupt
                                            // Write 1 to clear.
#define MSC_IFLG_SDIO           0x00000080  // Indicates whether the interrupt from SDIO is detected.
                                            // 0: The interrupt from SDIO is not detected.
                                            // 1: The interrupt from SDIO is detected.
#define MSC_IFLG_TXFIFOWRREQ    0x00000040  // Transmit FIFO write request. Set if data FIFO becomes half empty.(the number of words is < 8)
                                            // 0: No request for data Write to MSC_TXFIFO
                                            // 1: Request for data write to MSC_TXFIFO
#define MSC_IFLG_RXFIFORDREQ    0x00000020  // Receive FIFO read request. Set if data FIFO becomes half full(the number of words is >= 8) or the entries in data FIFO are the last read data.
                                            // 0: No request for data read from MSC_RXFIFO
                                            // 1: Request for data read from MSC_RXFIFO
#define MSC_IFLG_ENDCMDRES      0x00000004  // Indicates whether the command/response sequence has been finished.
                                            // 0: The command/response sequence has not been finished.
                                            // 1: The command/response sequence has been finished.
                                            // Write 1 to clear.
#define MSC_IFLG_PRGDONE        0x00000002  // Indicate whether card has finished programming.
                                            // 0: Card has not finished programming and is busy.
                                            // 1: Card has finished programming and is no longer busy.
                                            // Write 1 to clear.
#define MSC_IFLG_DATATRANDONE   0x00000001  // Indicate whether data transfer is done.
                                            // Note that for stream read/write, only when CMD12(STOP_TRANS) has been sent, is this bit set.
                                            // 0: Data transfer is not complete
                                            // 1: Data transfer has completed or an error has occurred.
                                            // Write 1 to clear.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_CMD register.
//
//*****************************************************************************
#define MSC_CMD_CMDINDEX        0x0000003F  // Specifies the command index to be executed.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_ARG register.
//
//*****************************************************************************
#define MSC_ARG_ARG             0xFFFFFFFF  // Specifies the argument for the current command

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_RES register.
//
//*****************************************************************************
#define MSC_RES_DATA            0x0000FFFF  // Contains the response to the command that is sent by the MMC/SD controller.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_RXFIFO register.
//
//*****************************************************************************
#define MSC_RXFIFO_DATA         0xFFFFFFFF  // One word of read data.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_TXFIFO register.
//
//*****************************************************************************
#define MSC_TXFIFO_DATA         0xFFFFFFFF  // One word of write data.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_LPM register.
//
//*****************************************************************************
#define MSC_LPM_DRVSEL          0xC0000000  // Drive clock selection
                                            // 00: CMD and DATA are driven by MSC_CLK failing edge.
                                            // 01: CMD and DATA are drivern by clock rising edge which is 1ns delayed by MSC_CLK
                                            // 10: CMD and DATA are driven by cock rising edge which 1/4 phase delayed by MSC_CLK.
                                            // 11: Reserved.
                                            // When this field is not zero, MSC_CLKRT.CLK_RATE must be set to 0.
#define MSC_LPM_SMPSEL          0x20000000  // Sample clock selection.
                                            // 0: CMD and DATA are sampled by MSC_CLK rising edge.
                                            // 1: CMD and DATA are sampled by clock rising edge which is 1/4 or 1/2 phase delayed by MSC_CLK.
                                            // When this field is 1, MSC_CLKRT.CLK_RATE must be set to 0.
#define MSC_LPM_LPM             0x00000001  // Low Power Mode Enable
                                            // 0: Disable low powe mode
                                            // 1: Enable low power mode
                                            // Clock will stop when card in idle(should be normally set to only MMC and SD cards. For SDIO cards, if interrupts must be detected, clock should not be stopped).
                                            // When software sets the bit, MSC clock can auto be stopped.
                                            // Note: when set the bit, the clock start and stop can be not use.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_DMAC register.
//
//*****************************************************************************
#define MSC_DMAC_MODESEL        0x00000080  // Transfer mode selection.
                                            // 0: Do not specify the transfer length. This is the standard transfer.
                                            // 1: Specify the transfer length. If this mode is selected, the transfer performace can be improved.
#define MSC_DMAC_AOFST          0x00000060  // Address Offset. This field is effective only when ALIGNEN is 1b.
                                            // 00b: The lowest 2 bit of data transer start address is 00b.
                                            // 01b: The lowest 2 bit of data transfer start address is 01b.
                                            // 10b: The lowest 2 bit of data transfer start address is 10b.
                                            // 11b: The lowest 2 bit of data transfer start address is 11b.
#define MSC_DMAC_ALIGNEN        0x00000010  // Align Enable.
                                            // 0: Only world boundary data transfer is supported.
                                            // 1: Byte boundary data transfer is supported.
#define MSC_DMAC_INCR           0x000000C0  // Burst type selection.
                                            // The field only effects to special DMA in MSC controller.
                                            // 2'b00: The burst type of DMA is INCR16.
                                            // 2'b01: The burst type of DMA is INCR32.
                                            // 2'b10: The burst type of DMA is INCR64.
#define MSC_DMAC_DMASEL         0x00000002  // One of supported DMA modes is selected.
                                            // 0: Special DMA in MSC controller is used.
                                            // 1: Common DMA in DMAC controller is used.
#define MSC_DMAC_DMAEN          0x00000001  // DMA Function Enable.
                                            // 0: Disable DMA function.
                                            // 1: Enable DMA function.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_DMANDA register.
//
//*****************************************************************************
#define MSC_DMANDA_NDA          0xFFFFFFFF  // SDMA Next Descriptro Physical Address.
                                            // The SDMA will change the register when a descriptor is read in. It should be 4-word aligned.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_DMADA register.
//
//*****************************************************************************
#define MSC_DMADA_DA            0xFFFFFFFF  // SDMA Data Physical Address.
                                            // The data address of the current descriptor will be copied to this field.
                                            // The SDMA will increment it during the data transfer automatically.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_DMALEN register.
//
//*****************************************************************************
#define MSC_DMALEN_LEN          0xFFFFFFFF  // SDMA Data Length to be transferred. Its unit is byte.
                                            // The data length of the current descriptor will be copied to this field.
                                            // The SDMA will decrement it during the data transfer automatically.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_DMACMD register.
//
//*****************************************************************************
#define MSC_DMACMD_IDI          0xFF000000  // Identification of the interrupt DMA.
                                            // This filed will copy the ID when a DMA transfer interrupt occurs.
                                            // If the interrupt of DMA is disabled, the field will not change.
#define MSC_DMACMD_ID           0x00FF0000  // Identification of Current DMA Transfer.
#define MSC_DMACMD_OFFSET       0x00000600  // Address Offset
                                            // This field is effective when ALIGN_EN is set to 1b.
                                            // 00: data address[1:0] is 00b.
                                            // 01: data address[1:0] is 01b.
                                            // 10: data address[1:0] is 10b.
                                            // 11: data address[1:0] is 11b.
#define MSC_DMACMD_ALIGNEN      0x00000100  // Address Align Enable.
                                            // This bit should be set to 1b if the data address is not word-aligned.
#define MSC_DMACMD_ENDI         0x00000002  // Interrupt Enable for Current DMA Transfer End.
                                            // 0: Disable interrupt.
                                            // 1: Enable interrupt.
#define MSC_DMACMD_LINK         0x00000001  // Control the end of DMA Descriptor.
                                            // 0: DMA will go to idle state after the current transmission is finished.
                                            //    The MSC_STAT.DMAEND will be set to 1b when the current read/write is finished. If the interrupt mask bit MSC_IMASK.DMAEND is 0b, the interrupt flag MSC_IFLG.DMAEND will be set to 1b also.
                                            // 1: DMA will continue to fetch another descriptor.

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_CTRL2 register.
//
//*****************************************************************************
#define MSC_CTRL2_PIP           0x1F000000  // Pin Level Interrupt Polarity
                                            // 0: Interrupt can be trigged when pin level is low
                                            // 1: Interrupt can be trigged when pin level is high
#define MSC_CTRL2_RESERVED1     0x00800000  // Inner usage, keep to 1.
#define MSC_CTRL2_STPRM         0x00000010  // Stop Read Operation Mode Seletion.
                                            // 0: Host can stop read operation during data transfer.
                                            // 1: Host only can stop read operation during block gap.
#define MSC_CTRL2_RESERVED0     0x00000008  // Inner usage, keep to 0.
#define MSC_CTRL2_SMS           0x00000007  // Speed Mode Selection.
                                            // 000: Default speed.
                                            // 001: High speed.
                                            // 010: SDR12
                                            // 011: SDR25
                                            // 100: SDR50
                                            // others: Reserved

//*****************************************************************************
//
// The following are defines for the bit fields in the MSC_O_RTCNT register.
//
//*****************************************************************************
#define MSC_RTCNT_RTCNT         0xFFFFFFFF  // This field indicates how many data in word units are stored in RTFIFO.

#endif /* __HW_MSC_H__ */
