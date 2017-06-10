//*****************************************************************************
//
// hw_tcu.h - Macros used when accessing the TCU hardware.
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

#ifndef __HW_TCU_H__
#define __HW_TCU_H__

//*****************************************************************************
//
// The following are defines for the TCU register offsets.
//
//*****************************************************************************
#define TCU_O_ER                0x00000010  // Timer Counter Enable Register(16-bit)
#define TCU_O_ESR               0x00000014  // Timer Counter Enable Set Register
#define TCU_O_ECR               0x00000018  // Timer Counter Enable Clear Register
#define TCU_O_SR                0x0000001C  // Timer Stop Register
#define TCU_O_FR                0x00000020  // Timer Flag Register
#define TCU_O_FSR               0x00000024  // Time Flag Set Register
#define TCU_O_FCR               0x00000028  // Timer Flag Clear Register
#define TCU_O_SSR               0x0000002C  // Timer Stop Set Register
#define TCU_O_MR                0x00000030  // Timer Mask Register
#define TCU_O_MSR               0x00000034  // Timer Mask Set Register
#define TCU_O_MCR               0x00000038  // Timer Mask Clear Register
#define TCU_O_SCR               0x0000003C  // Timer Stop Clear Register
#define TCU_O_DFR0              0x00000040  // Timer Data Full Register(16-bit)
#define TCU_O_DHR0              0x00000044  // Timer Data Half Register 0(16-bit)
#define TCU_O_CNT0              0x00000048  // Timer Counter 0(16-bit)
#define TCU_O_CSR0              0x0000004C  // Timer Control Register 0(16-bit)
#define TCU_O_DFR1              0x00000050  // Timer Data Full Register 1(16-bit)
#define TCU_O_DHR1              0x00000054  // Timer Data Half Register 1(16-bit)
#define TCU_O_CNT1              0x00000058  // Timer Counter 1(16-bit)
#define TCU_O_CSR1              0x0000005C  // Timer Control Register 1(16-bit)
#define TCU_O_DFR2              0x00000060  // Timer Data Full Register 2(16-bit)
#define TCU_O_DHR2              0x00000064  // Timer Data Half Register 2(16-bit)
#define TCU_O_CNT2              0x00000068  // Timer Counter 2(16-bit)
#define TCU_O_CSR2              0x0000006C  // Timer Control Register 2(16-bit)
#define TCU_O_DFR3              0x00000070  // Timer Data Full Register 3(16-bit)
#define TCU_O_DHR3              0x00000074  // Timer Data Half Register 3(16-bit)
#define TCU_O_CNT3              0x00000078  // Timer Counter 3(16-bit)
#define TCU_O_CSR3              0x0000007C  // Timer Control Register 3(16-bit)
#define TCU_O_DFR4              0x00000080  // Timer Data Full Register 4(16-bit)
#define TCU_O_DHR4              0x00000084  // Timer Data Half Register 4(16-bit)
#define TCU_O_CNT4              0x00000088  // Timer Counter 4(16-bit)
#define TCU_O_CSR4              0x0000008C  // Timer Control Register 4(16-bit)
#define TCU_O_DFR5              0x00000090  // Timer Data Full Register 5(16-bit)
#define TCU_O_DHR5              0x00000094  // Timer Data Half Register 5(16-bit)
#define TCU_O_CNT5              0x00000098  // Timer Counter 5(16-bit)
#define TCU_O_CSR5              0x0000009C  // Timer Control Register 5(16-bit)
#define TCU_O_DFR6              0x000000A0  // Timer Data Full Register 6(16-bit)
#define TCU_O_DHR6              0x000000A4  // Timer Data Half Register 6(16-bit)
#define TCU_O_CNT6              0x000000A8  // Timer Counter 6(16-bit)
#define TCU_O_CSR6              0x000000AC  // Timer Control Register 6(16-bit)
#define TCU_O_DFR7              0x000000B0  // Timer Data Full Register 7(16-bit)
#define TCU_O_DHR7              0x000000B4  // Timer Data Half Register 7(16-bit)
#define TCU_O_CNT7              0x000000B8  // Timer Counter 7(16-bit)
#define TCU_O_STR               0x000000F0  // Timer Status Register
#define TCU_O_STSR              0x000000F4  // Timer Status Set Register
#define TCU_O_STCR              0x000000F8  // Timer Status Clear Register
#define TCU_O_MODE0             0x00000100  // Timer Control mode register 0(16-bit)
#define TCU_O_FWD0              0x00000104  // Timer fifo write data register 0(16-bit)
#define TCU_O_FIFOSR0           0x00000108  // Timer fifo state register 0
#define TCU_O_MODE3             0x00000110  // Timer Control mode register 3(16-bit)
#define TCU_O_FWD3              0x00000114  // Timer fifo write data register 3(16-bit)
#define TCU_O_FIFOSR3           0x00000118  // Timer fifo state register 3
#define TCU_O_MODE4             0x00000120  // Timer Control mode register 4(16-bit)
#define TCU_O_FWD4              0x00000124  // Timer fifo write data register 4(16-bit)
#define TCU_O_FIFOSR4           0x00000128  // Timer fifo state register 4
#define TCU_O_MODE5             0x00000130  // Timer Control mode register 5(16-bit)
#define TCU_O_FWD5              0x00000134  // Timer fifo write data register 5(16-bit)
#define TCU_O_FIFOSR5           0x00000138  // Timer fifo state register 5

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_ER register.
//
//*****************************************************************************
#define TCU_ER_OSTEN            0x00008000  // Enable status of the counter in OST.
                                            // 0: Stop counting up
                                            // 1: Begin counting up
#define TCU_ER_TCEN7            0x00000080  // Enable status of the counter in timer 7.
#define TCU_ER_TCEN6            0x00000040  // Enable status of the counter in timer 6.
#define TCU_ER_TCEN5            0x00000020  // Enable status of the counter in timer 5.
#define TCU_ER_TCEN4            0x00000010  // Enable status of the counter in timer 4.
#define TCU_ER_TCEN3            0x00000008  // Enable status of the counter in timer 3.
#define TCU_ER_TCEN2            0x00000004  // Enable status of the counter in timer 2.
#define TCU_ER_TCEN1            0x00000002  // Enable status of the counter in timer 1.
#define TCU_ER_TCEN0            0x00000001  // Enable status of the counter in timer 0.

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_ESR register.
//
//*****************************************************************************
#define TCU_ESR_OSTST            0x00008000  // Enable the counter in OST.
#define TCU_ESR_TCST7            0x00000080  // Enable the counter in timer 7.
#define TCU_ESR_TCST6            0x00000040  // Enable the counter in timer 6.
#define TCU_ESR_TCST5            0x00000020  // Enable the counter in timer 5.
#define TCU_ESR_TCST4            0x00000010  // Enable the counter in timer 4.
#define TCU_ESR_TCST3            0x00000008  // Enable the counter in timer 3.
#define TCU_ESR_TCST2            0x00000004  // Enable the counter in timer 2.
#define TCU_ESR_TCST1            0x00000002  // Enable the counter in timer 1.
#define TCU_ESR_TCST0            0x00000001  // Enable the counter in timer 0.

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_ECR register.
//
//*****************************************************************************
#define TCU_ESR_OSTCL            0x00008000  // Disable the counter in OST.
#define TCU_ESR_TCCL7            0x00000080  // Disable the counter in timer 7.
#define TCU_ESR_TCCL6            0x00000040  // Disable the counter in timer 6.
#define TCU_ESR_TCCL5            0x00000020  // Disable the counter in timer 5.
#define TCU_ESR_TCCL4            0x00000010  // Disable the counter in timer 4.
#define TCU_ESR_TCCL3            0x00000008  // Disable the counter in timer 3.
#define TCU_ESR_TCCL2            0x00000004  // Disable the counter in timer 2.
#define TCU_ESR_TCCL1            0x00000002  // Disable the counter in timer 1.
#define TCU_ESR_TCCL0            0x00000001  // Disable the counter in timer 0.

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_SR register.
//
//*****************************************************************************
#define TCU_SR_WDTS             0x00010000  // 0: The clock supplies to WDT is supplied
                                            // 1: The clock supplies to WDT is stopped
#define TCU_SR_OSTS             0x00008000  // OST.
#define TCU_SR_STOP7            0x00000080  // timer 7.
#define TCU_SR_STOP6            0x00000040  // timer 6.
#define TCU_SR_STOP5            0x00000020  // timer 5.
#define TCU_SR_STOP4            0x00000010  // timer 4.
#define TCU_SR_STOP3            0x00000008  // timer 3.
#define TCU_SR_STOP2            0x00000004  // timer 2.
#define TCU_SR_STOP1            0x00000002  // timer 1.
#define TCU_SR_STOP0            0x00000001  // timer 0.

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_FR register.
//
//*****************************************************************************
#define TCU_FR_FIFOEFLAG5       0x08000000  // FIFO empty flag 5
                                            // 0: fifo not empty
                                            // 1: fifo empty
#define TCU_FR_FIFOEFLAG4       0x04000000  // FIFO empty flag 4
#define TCU_FR_FIFOEFLAG3       0x02000000  // FIFO empty flag 3
#define TCU_FR_FIFOEFLAG0       0x01000000  // FIFO empty flag 0
#define TCU_FR_HFLAG7           0x00800000  // timer 7 half comparison match flag. (TCU_O_CNTx = TCU_O_DHRx)
                                            // 0: Comparison not match
                                            // 1: Comparison match
#define TCU_FR_HFLAG6           0x00400000  // timer 6 half comparison match flag
#define TCU_FR_HFLAG5           0x00200000  // timer 5 half comparison match flag
#define TCU_FR_HFLAG4           0x00100000  // timer 4 half comparison match flag
#define TCU_FR_HFLAG3           0x00080000  // timer 3 half comparison match flag
#define TCU_FR_HFLAG2           0x00040000  // timer 2 half comparison match flag
#define TCU_FR_HFLAG1           0x00020000  // timer 1 half comparison match flag
#define TCU_FR_HFLAG0           0x00010000  // timer 0 half comparison match flag
#define TCU_FR_OSTFLAG          0x00008000  // OST comparison match flag
#define TCU_FR_FIFOFLAG5        0x00000800  // FIFO comparison match flag. (TCU_O_CNTx = TCU_O_FWDx)
#define TCU_FR_FIFOFLAG4        0x00000400  //
#define TCU_FR_FIFOFLAG3        0x00000200  //
#define TCU_FR_FIFOFLAG0        0x00000100  //
#define TCU_FR_FFLAG7           0x00000080  // timer 7 FULL comparison match flag. (TCU_O_CNTx = TCU_O_DFRx)
#define TCU_FR_FFLAG6           0x00000040  // timer 6
#define TCU_FR_FFLAG5           0x00000020  // timer 5
#define TCU_FR_FFLAG4           0x00000010  // timer 4
#define TCU_FR_FFLAG3           0x00000008  // timer 3
#define TCU_FR_FFLAG2           0x00000004  // timer 2
#define TCU_FR_FFLAG1           0x00000002  // timer 1
#define TCU_FR_FFLAG0           0x00000001  // timer 0

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_FSR register.
//
//*****************************************************************************
#define TCU_FSR_FIFOESET5       0x08000000  // Set TCU_FR_FIFOEFLAGx
#define TCU_FSR_FIFOESET4       0x04000000  //
#define TCU_FSR_FIFOESET3       0x02000000  //
#define TCU_FSR_FIFOESET0       0x01000000  //
#define TCU_FSR_HFST7           0x00800000  // Set TCU_FR_HFLAGx
#define TCU_FSR_HFST6           0x00400000  //
#define TCU_FSR_HFST5           0x00200000  //
#define TCU_FSR_HFST4           0x00100000  //
#define TCU_FSR_HFST3           0x00080000  //
#define TCU_FSR_HFST0           0x00010000  //
#define TCU_FSR_OSTFST          0x00008000  // Set TCU_FR_OSTFLAG
#define TCU_FSR_FIFOST5         0x00000800  // Set TCU_FR_FIFOFLAGx
#define TCU_FSR_FIFOST4         0x00000400  //
#define TCU_FSR_FIFOST3         0x00000300  //
#define TCU_FSR_FIFOST0         0x00000100  //
#define TCU_FSR_FFST7           0x00000080  // Set TCU_FR_FFLAGx
#define TCU_FSR_FFST6           0x00000040  //
#define TCU_FSR_FFST5           0x00000020  //
#define TCU_FSR_FFST4           0x00000010  //
#define TCU_FSR_FFST3           0x00000008  //
#define TCU_FSR_FFST0           0x00000001  //

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_FCR register.
//
//*****************************************************************************
#define TCU_FCR_FIFOECL5        0x08000000  // Clear TCU_FR_FIFOEFLAGx
#define TCU_FCR_FIFOECL4        0x04000000  //
#define TCU_FCR_FIFOECL3        0x02000000  //
#define TCU_FCR_FIFOECL0        0x01000000  //
#define TCU_FCR_HFCL7           0x00800000  // Clear TCU_FR_HFLAGx
#define TCU_FCR_HFCL6           0x00400000  //
#define TCU_FCR_HFCL5           0x00200000  //
#define TCU_FCR_HFCL4           0x00100000  //
#define TCU_FCR_HFCL3           0x00080000  //
#define TCU_FCR_HFCL2           0x00040000  //
#define TCU_FCR_HFCL1           0x00020000  //
#define TCU_FCR_HFCL0           0x00010000  //
#define TCU_FCR_OSTFCL          0x00008000  // Clear TCU_FR_OSTFLAG
#define TCU_FCR_FIFOCL5         0x00000800  // Clear TCU_FR_FIFOFLAGx
#define TCU_FCR_FIFOCL4         0x00000400  //
#define TCU_FCR_FIFOCL3         0x00000200  //
#define TCU_FCR_FIFOCL0         0x00000100  //
#define TCU_FCR_FFCL7           0x00000080  // Clear TCU_FR_FFLAGx
#define TCU_FCR_FFCL6           0x00000040  //
#define TCU_FCR_FFCL5           0x00000020  //
#define TCU_FCR_FFCL4           0x00000010  //
#define TCU_FCR_FFCL3           0x00000008  //
#define TCU_FCR_FFCL2           0x00000004  //
#define TCU_FCR_FFCL1           0x00000002  //
#define TCU_FCR_FFCL0           0x00000001  //

//*****************************************************************************
//
// The following are defines for the bit fields in the TCU_O_SSR register.
//
//*****************************************************************************
#define TCU_SSR_WDTSS           0x00010000  // Set TCU_SR_WDTS
#define TCU_SSR_OSTSS           0x00008000  // Set TCU_SR_OSTS
#define TCU_SSR_STPS7           0x00000080  // Set TCU_SR_STOPx


#endif /* __HW_TCU_H__ */

