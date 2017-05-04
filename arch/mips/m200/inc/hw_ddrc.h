//*****************************************************************************
//
// hw_ddrc.h - Macros used when accessing the DDR Controller
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

#ifndef __DDRC_H__
#define __DDRC_H__

//*****************************************************************************
//
// The following are defines for the DDRC register offsets.
//
//*****************************************************************************
#define DDRC_O_STATUS           0x00000000  // DDRC Status Register
#define DDRC_O_CFG              0x00000004  // DDRC Configure Register
#define DDRC_O_CTRL             0x00000008  // DDR General Control Register
#define DDRC_O_LMR              0x0000000C  // DDR Load-Mode-Register
#define DDRC_O_TIMING1          0x00000060  // DDR Timing Configure Register 1
#define DDRC_O_TIMING2          0x00000064  // DDR Timing Configure Register 2
#define DDRC_O_TIMING3          0x00000068  // DDR Timing Configure Register 3
#define DDRC_O_TIMING4          0x0000006C  // DDR Timing Configure Register 4
#define DDRC_O_TIMING5          0x00000070  // DDR Timing Configure Register 5
#define DDRC_O_TIMING6          0x00000074  // DDR Timing Configure Register 6
#define DDRC_O_REFCNT           0x00000018  // Auto-Refresh Counter
#define DDRC_O_MMAP0            0x00000024  // DRAM CS0 Map Configure Register
#define DDRC_O_MMAP1            0x00000028  // DRAM CS1 Map Configure Register
#define DDRC_O_DLP              0x000000BC  // DFI lower power handshake register
#define DDRC_O_REMAP1           0x0000009C  // DDR address remapping register1
#define DDRC_O_REMAP2           0x000000A0  // DDR address remapping register2
#define DDRC_O_REMAP3           0x000000A4  // DDR address remapping register3
#define DDRC_O_REMAP4           0x000000A8  // DDR address remapping register4
#define DDRC_O_REMAP5           0x000000AC  // DDR address remapping register5
#define DDRC_O_WCMDCTRL1        0x00000100  // Write command reorder & grouping
#define DDRC_O_RCMDCTRL0        0x00000104  // Read Channel mode control
#define DDRC_O_RCMDCTRL1        0x00000108  // Read Channel mode control
#define DDRC_O_WDATTHD0         0x00000114  // Wdata Channel mode control
#define DDRC_O_WDATTHD1         0x00000118  // Wdata Channel mode control
#define DDRC_O_IPORTWPRI        0x00000240  // Configuration of Internal Priority for write channel
#define DDRC_O_IPORTRPRI        0x00000244  // Configuration of Internal Priority for read channel
#define DDRC_O_CH0WDOS0         0x00000200  // WQoS configure for channel 0
#define DDRC_O_CH1WDOS          0x00000204  // WQoS configure for channel 1
#define DDRC_O_CH2WDOS          0x00000208  // WQoS configure for channel 2
#define DDRC_O_CH3WDOS          0x0000020C  // WQoS configure for channel 3
#define DDRC_O_CH4WDOS          0x00000210  // WQoS configure for channel 4
#define DDRC_O_CH5WDOS          0x00000214  // WQoS configure for channel 5
#define DDRC_O_CH6WDOS          0x00000218  // WQoS configure for channel 6
#define DDRC_O_CH0RDOS          0x00000220  // RQoS configure for channel 0
#define DDRC_O_CH1RDOS          0x00000224  // RQoS configure for channel 1
#define DDRC_O_CH2RDOS          0x00000228  // RQoS configure for channel 2
#define DDRC_O_CH3RDOS          0x0000022C  // RQoS configure for channel 3
#define DDRC_O_CH4RDOS          0x00000230  // RQoS configure for channel 4
#define DDRC_O_CH5RDOS          0x00000234  // RQoS configure for channel 5
#define DDRC_O_CH6RDOS          0x00000238  // RQoS configure for channel 6
#define DDRC_O_AUTOSR_CNT       0x00000308  // Auto Self Refresh conter configuration
#define DDRC_O_AUTOSR_EN        0x00000304  // Auto Self Refresh enable
#define DDRC_O_EFPRB_CFG        0x0000030C  // Bus efficiency probe configuration
#define DDRC_O_EFPRB_VALUE      0x00000310  // Bus efficiency data
#define DDRC_O_CLKSTP_CFG       0x00000068  // Auto Clock Gating configuration
#define DDRC_O_DRC_STATUS       0x0000006C  // DDR status
#define DDRC_O_PHYRET_CFG       0x00000034  // Retention Mode Configuation
#define DDRC_O_EPD_HADDR        0x00000070  // EPD wake up configure
#define DDRC_O_EPD_LADDR        0x00000074  // EPD wake up configure
#define DDRC_O_PHYRST_CFG       0x00000080  // DDR Phy reset control
#define DDRC_O_CFG0             0x00000084  // Clock change Configuration
#define DDRC_O_CFG1             0x00000088  // Clock change Configuration

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_STATUS register.
//
//*****************************************************************************
#define DDRC_STATUS_DFI_INIT    0x80000000  // DFI(PHY interface) initial status
#define DDRC_STATUS_ENDIAN      0x00000080  // Endian status
#define DDRC_STATUS_MISS        0x00000040  // Indicate the accessed address is out of memory range
#define DDRC_STATUS_DPDN        0x00000020  // Indicate the deep-power-down status of DDR memory
#define DDRC_STATUS_PDN         0x00000010  // Indicate the power-down status os DDR memroy
#define DDRC_STATUS_AREF        0x00000008  // Indicate the auto-refresh status of DDR memory
#define DDRC_STATUS_SREF        0x00000004  // Indicate the self-refresh status of DDR memory
#define DDRC_STATUS_CKE1        0x00000002  // Indicate the status of CKE1 pin(only usable after DDR PHY initialized)
#define DDRC_STATUS_CKE0        0x00000001  // Indicate the status of CKE0 pin(only usable after DDR PHY initialized)

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_CFG register.
//
//*****************************************************************************
#define DDRC_CFG_ROW1           0x38000000  // Row Address Width. Specify the row address width of external DDR
#define DDRC_CFG_COL1           0x07000000  // Column Address Width. Specify the column address width of external DDR
#define DDRC_CFG_BA1            0x00800000  // 0: 4 bank device, 1: 8 bank device
#define DDRC_CFG_IMBA           0x00400000  // 0: CS0, CS1 connected 2 memory chips which have same ROW,COL,BA configuration
                                            //    In this mode, both chips are configured by ROW0,COL0,BA0
                                            //    ROW1,COL1,BA1 does not effect the system
                                            // 1: CS0, CS1 connected 2 memory chips which have different ROW, COL,BA configuration
                                            //    Chip0 is configured by ROW0,COL0,BA0
                                            //    Chip1 is configured by ROW1,COL1,BA1
#define DDRC_CFG_TYPE           0x000E0000  // Select external memory device type
#define DDRC_CFG_ODTEN          0x00010000  // whether enable ODT
#define DDRC_CFG_MISPE          0x00008000  // Missing CS protection Enable
#define DDRC_CFG_RAW0           0x00003800  // Row Address Width. Specify the row address width of external DDR
#define DDRC_CFG_COL0           0x00000700  // Column Address Width. Specify the column address width of external DDR
#define DDRC_CFG_CS1EN          0x00000080  // DDR Chip-Select-1 Enable
#define DDRC_CFG_CS0EN          0x00000040  // DDR Chip-Select-0 Enable
#define DDRC_CFG_CL             0x0000003C  // CAS Latency. Not in use for this version
#define DDRC_CFG_BA0            0x00000002  // Bank Address Width of DDR memory
#define DDRC_CFG_DW             0x00000001  // External DDR Memory Data Width. 0: 16-bit, 1: 32-bit

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_CTRL register.
//
//*****************************************************************************
#define DDRC_CTRL_DFI_RST       0x00800000  // Reserved, please keep the default value
#define DDRC_CTRL_DLL_RST       0x00400000  // Reserved, please keep the default value
#define DDRC_CTRL_CTL_RST       0x00200000  // Reserved, please keep the default value
#define DDRC_CTRL_CFG_RST       0x00100000  // Reserved, please keep the default value
#define DDRC_CTRL_KEEPSR        0x00020000  // inner usage, keep to 0
#define DDRC_CTRL_ACTPD         0x00008000  // Active Power-Down
#define DDRC_CTRL_PDT           0x00007000  // Power-Down Timer
#define DDRC_CTRL_ACTSTP        0x00000800  // Active Clock-Stop
#define DDRC_CTRL_DPD           0x00000040  // Software drives external Mobile DDR device entering Deep-Power-Down mode
#define DDRC_CTRL_SR            0x00000020  // Software dreive external DDR device entering Self-Refresh mode
#define DDRC_CTRL_UNALIGN       0x00000010  // Enable unaligned transfer on AXI BUS. Not use in this version
#define DDRC_CTRL_ALH           0x00000008  // Advanced Latency Hiding. This is a test-oriented register
#define DDRC_CTRL_CKE           0x00000002  // Control the status of CKE pin
#define DDRC_CTRL_RESET         0x00000001  // Module reset for ddr_controller

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_LMR register.
//
//*****************************************************************************
#define DDRC_LMR_DDR_ADDR       0xFFFFF000  // When performing a DDR command, DDR_ADDR[13:0] corresponding to 
                                            // external DDR address Pin A[13:0], DDR_ADDR[15:14] are reserved
#define DDRC_LMR_BA             0x00000700  // Bank Address. When performing a DDR command, BA[2:0] corresponding
                                            // external DDR address Pin BA[2:0]
#define DDRC_LMR_CMD            0x00000038  // Select command to process when setting START from low to high
#define DDRC_LMR_START          0x00000001  // Start to perform a command to external DDR memory

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING1 register.
//
//*****************************************************************************
#define DDRC_TIMING1_tRTP       0x3C000000  // READ to PRECHARGE command period
#define DDRC_TIMING1_tWTR       0x003F0000  // WRITE to READ command period
#define DDRC_TIMING1_tWR        0x00003F00  // WRITE Recovoery Time defined by register MR of DDR2 DDR3 memory
#define DDRC_TIMING1_tWL        0x0000003F  // Write latency, please notice that this version only support AL=0

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING2 register.
//
//*****************************************************************************
#define DDRC_TIMING2_tCCD       0x3F000000  // CAS# to CAS# command delay
#define DDRC_TIMING2_tRAS       0x003F0000  // ACTIVE to PRECHARGE command period
                                            // tRAS defines the ACTIVE to PRECHARGE command period to the same bank
#define DDRC_TIMING2_tRCD       0x00003F00  // ACTIVE to READ or WRITE command period
                                            // tRCD defines the ACTIVE to READ/WRITE command period to the same bank
#define DDRC_TIMING2_tRL        0x0000003F  // Read latency, please notice that htis version only support AL=0

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING3 register.
//
//*****************************************************************************
#define DDRC_TIMING3_ONUM       0x78000000  // Keep to 4 in this version
#define DDRC_TIMING3_tCKSRE     0x07000000  // Valid clock after enter self-refresh(tCKSRX = tCKSRE in this version)
                                            // Delay time is tCKSRE * 8(tCK)
#define DDRC_TIMING3_tRP        0x003F0000  // PRECHARGE command period.
                                            // tRP defines the PRECHARGE to next command period to the same bank
#define DDRC_TIMING3_tRRD       0x00003F00  // ACTIVE bank A to ACTIVE bank B command period
                                            // tRRD defines the ACTIVE to ACTIVE command period to different banks
#define DDRC_TIMING3_tRC        0x0000003F  // ACTIVE to ACTIVE command period
                                            // tRC defines the ACTIVE to ACTIVE command period to the same bank

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING4 register.
//
//*****************************************************************************
#define DDRC_TIMING4_tRFC       0x3F000000  // AUTO-REFRESH command period
                                            // tRFC defines the minimum delay after an AUTO-REFRESH command.
                                            // During tRFC period, no command can be issued to DDR memory
                                            // Delay Time = 2 * (tRFC + 1)(tCK))
#define DDRC_TIMING4_tEXTRW     0x00C00000  // keep the default value
#define DDRC_TIMING4_tRWCOV     0x00180000  // keep the default value
#define DDRC_TIMING4_tCKE       0x00070000  // minimum CKE pulse width
                                            // tCKE defines the minimum CKE pulse width, include high level and low level
                                            // Delay Time is (tCKE + 1)(tCK)
#define DDRC_TIMING4_tMINSR     0x00000F00  // Minimum Self-Refresh/Deep-Power-Down time
                                            // After DDR memory turns into Self-Refresh or Deep-Power-Down mode,
                                            // it will NOT exit until tMINSR condition meets
                                            // Delay Time = (tMINSR * 8 + 1)(tCK)
#define DDRC_TIMING4_tXP        0x00000070  // EXIT-POWER-DOWN to next valid command period
                                            // tXP defines the EXIT-POWER-DOWN to next valid command period to all banks
                                            // Delay Time is (tXP + 1)(tCK)
#define DDRC_TIMING4_tWRD       0x00000003  // Load-Mode-Register to next valid command period
                                            // tMRD defines the Load-Mode-Register to next valid command period
                                            // Delay Time = (tWRD + 1)(tCK)

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING5 register.
//
//*****************************************************************************
#define DDRC_TIMING5_tCTLUPD    0xFF000000  // Inner usage. Not need to change
#define DDRC_TIMING5_tRTW       0x003F0000  // Read to Write latency
#define DDRC_TIMING5_tRDLAT     0x00003F00  // tRL-2 (When use LPDDR2, set tRDLAT=tRL)
#define DDRC_TIMING5_tWDLAT     0x0000003F  // tWl-1 (When use LPDDR2, set tWDLAT=tWL)

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_TIMING6 register.
//
//*****************************************************************************
#define DDRC_TIMING6_tXSRD      0xFF000000  // Exit self-refresh to READ delay
#define DDRC_TIMING6_tFAW       0x003F0000  // 4-active command window
#define DDRC_TIMING6_tCFGW      0x00003F00  // Write PHY configure registers to other commands delay
                                            // Not need to change
#define DDRC_TIMING6_tCFGR      0x0000003F  // Ready PHY configure registers to other commands delay
                                            // Not need to change

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REFCNT register.
//
//*****************************************************************************
#define DDRC_REFCNT_CON         0x00FF0000  // A constant value used to compare with the CNT value
#define DDRC_REFCNT_CNT         0x0000FF00  // 8-bit counter; When the value of CNT match the value of
                                            // CON, flag bit EQU is set high and an auto-refresh
                                            // command will be issued to DDR memory
#define DDRC_REFCNT_CLK_DIV     0x0000000E  // Clock Divider. Divide the dclk to generate a lower frequency
                                            // of clock to drive the auto-refresh counter. dclk is CKO clock
#define DDRC_REFCNT_REF_EN      0x00000001  // Enable Refresh Counter

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_MMAP0 register.
// ( AXI_BUS_Address[31:24] & MASK[7:0] ) == BASE
//
//*****************************************************************************
#define DDRC_MMAP0_BASE         0x0000FF00  // base address for chip0
#define DDRC_MMAP0_MASK         0x000000FF  // address mask for chip0

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_MMAP1 register.
// ( AXI_BUS_Address[31:24] & MASK[7:0] ) == BASE
//
//*****************************************************************************
#define DDRC_MMAP1_BASE         0x0000FF00  // base address for chip1
#define DDRC_MMAP1_MASK         0x000000FF  // address mask for chip1

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_DLP register.
//
//*****************************************************************************
#define DDRC_DLP_ONLY           0x00040000  // Inner usage, keep to 0
#define DDRC_DLP_UAR            0x00020000  // Inner usage, keep to 0
#define DDRC_DLP_ZQ             0x00010000  // Inner usage, keep to 0
#define DDRC_DLP_SLP            0x0000F000  // slow LP handshake timing register
                                            // Just keep it as reset value 0xF
#define DDRC_DLP_FLP            0x00000F00  // fast LP handshake timing register
                                            // Just keep it as reset value 0x0
#define DDRC_DLP_FSR            0x00000004  // 0: Use slow LP handshake process during self-refresh
                                            // 1: Use fast LP handshake process during self-refresh
#define DDRC_DLP_FPD            0x00000002  // 0: Use slow LP handshake process during power down
                                            // 1: Use fast LP handshake process during power down
#define DDRC_DLP_LPEN           0x00000001  // DFI low power(LP) interface enable

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REMAP1 register.
// BITxMP == address[31:12], address[11:0] reserved without any change
// usage: remap address[27:25] and address[14:12]
//        BIT15MP = 2
//        BIT14MP = 1
//        BIT13MP = 0
//        BIT2MP  = 15
//        BIT1MP  = 14
//        BIT0MP  = 13
//*****************************************************************************
#define DDRC_REMAP1_BIT3MP      0x1F000000  // remap address[15]
#define DDRC_REMAP1_BIT2MP      0x001F0000  // remap address[14]
#define DDRC_REMAP1_BIT1MP      0x00001F00  // remap address[13]
#define DDRC_REMAP1_BIT0MP      0x0000001F  // remap address[12]

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REMAP2 register.
//
//*****************************************************************************
#define DDRC_REMAP2_BIT7MP      0x1F000000  // remap address[19]
#define DDRC_REMAP2_BIT6MP      0x001F0000  // remap address[18]
#define DDRC_REMAP2_BIT5MP      0x00001F00  // remap address[17]
#define DDRC_REMAP2_BIT4MP      0x0000001F  // remap address[16]

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REMAP3 register.
//
//*****************************************************************************
#define DDRC_REMAP3_BIT11MP     0x1F000000  // remap address[23]
#define DDRC_REMAP3_BIT10MP     0x001F0000  // remap address[22]
#define DDRC_REMAP3_BIT9MP      0x00001F00  // remap address[21]
#define DDRC_REMAP3_BIT8MP      0x0000001F  // remap address[20]

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REMAP4 register.
//
//*****************************************************************************
#define DDRC_REMAP4_BIT15MP     0x1F000000  // remap address[27]
#define DDRC_REMAP4_BIT14MP     0x001F0000  // remap address[26]
#define DDRC_REMAP4_BIT13MP     0x00001F00  // remap address[25]
#define DDRC_REMAP4_BIT12MP     0x0000001F  // remap address[24]

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_REMAP5 register.
//
//*****************************************************************************
#define DDRC_REMAP5_BIT19MP     0x1F000000  // remap address[31]
#define DDRC_REMAP5_BIT18MP     0x001F0000  // remap address[30]
#define DDRC_REMAP5_BIT17MP     0x00001F00  // remap address[29]
#define DDRC_REMAP5_BIT16MP     0x0000001F  // remap address[28]

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_WCMDCTRL1 register.
//
//*****************************************************************************
#define DDRC_WCMDCTRL1_WLT      0xF8000000  // wcmd_lock_thd, Indicates how many
                                            // wcmd(write commands) are grouped together
                                            // Higher value improves total bandwidth of ddr, but leads to longer read command latency
#define DDRC_WCMDCTRL1_WHM      0x07F80000  // wpage_hit_max, Wpage-hit affects the reorder of wcmd, the wcmd with same page should be grouped
                                            // for higher performance. This register indicates the maximum number of wcmd to be bonded
                                            // Higher value improves total bandwidth of ddr, but leads to longer read maximum latency
#define DDRC_WCMDCTRL1_PQC      0x0007FF80  // pend_qos_cnt, Once the wcmd is accepted, a counter indicates how long the command is pending,
                                            // when the pending time is exceed Pend_QoS once, the priority of this command will increase 1, up to 3,
                                            // which increases the priority to be processed. This mechanism avoids the wcmd with lower priority penging too long
                                            // Dynamic configuration is not recommended
#define DDRC_WCMDCTRL1_WIU      0x0000007C  // warm_in_use, 0~12. Ram to be used in write command reorder
                                            // Configuration is not recommended
#define DDRC_WCMDCTRL1_DR       0x00000002  // disable_reorder, 0: wcmd will be reordered based on priority and pag-hit-conflict
                                            // 1: wcmd will not be reordered, the commands will be process with the order of being received
                                            // Dynamic configuration is not recommended
#define DDRC_WCMDCTRL1_MEW      0x00000001  // msk_empty_wfifo, 0: wcmd is involved in arbitration immediately
                                            // 1: wcmd is not involved in arbitration until receives enough wdata

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_RCMDCTRL0 register.
// rfifo_thd: read fifo threshold
// Read command is not involved in arbitration until the vacancy in rfifo is 
// higher than rfifo threshold. This should improves the DDR performance when
// processing accessing discrete pages. However for page-access(commands access
// the same page continuously) the performance will be lagged as the threshold
// affects the command grouping.
//
//*****************************************************************************
#define DDRC_RCMDCTRL0_C3RIC    0x80000000  // ch3_rcmd_igr_cflit, 0: disable
                                            // 1: enable. Consistence between wcmd and rcmd will not be guaranteed
#define DDRC_RCMDCTRL0_C3RFTE   0x40000000  // ch3_rfifo_thd_en, 0: disable rfifo_thd(recommended)
                                            // 1: enable rfifo_thd
#define DDRC_RCMDCTRL0_C3RFT    0x3F000000  // If Ch3 rfifo level threshold higher than this value, rcmd request is masked
#define DDRC_RCMDCTRL0_C2RIC    0x00800000  // ch2_rcmd_igr_cflit
#define DDRC_RCMDCTRL0_C2RFTE   0x00400000  // ch2_rfifo_thd_en
#define DDRC_RCMDCTRL0_C2RFT    0x003F0000  // ch2_rfifo_thd
#define DDRC_RCMDCTRL0_C0RIC    0x00000080  // ch0_rcmd_igr_cflit
#define DDRC_RCMDCTRL0_C0RFTE   0x00000040  // ch0_rfifo_thd_en
#define DDRC_RCMDCTRL0_C0RFT    0x0000003F  // ch0_rfifo_thd

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_RCMDCTRL1 register.
//
//*****************************************************************************
#define DDRC_RCMDCTRL1_C6RIC    0x00800000  // ch6_rcmd_igr_cflit
#define DDRC_RCMDCTRL1_C6RFTE   0x00400000  // ch6_rfifo_thd_en
#define DDRC_RCMDCTRL1_C6RFT    0x003F0000  // ch6_rfifo_thd
#define DDRC_RCMDCTRL1_C5RIC    0x00008000  // ch5_rcmd_igr_cflit
#define DDRC_RCMDCTRL1_C5RFTE   0x00004000  // ch5_rfifo_thd_en
#define DDRC_RCMDCTRL1_C5RFT    0x00003F00  // ch5_rfifo_thd
#define DDRC_RCMDCTRL1_C4RIC    0x00000080  // ch4_rcmd_igr_cflit
#define DDRC_RCMDCTRL1_C4RFTE   0x00000040  // ch4_rfifo_thd_en
#define DDRC_RCMDCTRL1_C4RFT    0x0000003F  // ch4_rfifo_thd

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_WDATTHD0 register.
// wfifo_thd: wdata fifo thresthold
// The threshold guarantees the wcmd will not be involved in arbitration until
// receive enough wdata. This improves DDR performance for AXI write access which
// wdata does not continuously transfer on bus. But for continuous transfer,
// performance would be worse.
//
//*****************************************************************************
#define DDRC_WDATTHD0_C3WFTE    0x40000000  // ch3_wfifo_thd_en, 1: Enabled
                                            // 0: Disabled(recommended)
#define DDRC_WDATTHD0_C3WFT     0x3F000000  // wcmd is involved in arbitration
                                            // before receiving all wdata
#define DDRC_WDATTHD0_C2WFTE    0x00400000  // ch2_wfifo_thd_en
#define DDRC_WDATTHD0_C2WFT     0x003F0000  // ch2_wfifo_thd
#define DDRC_WDATTHD0_C0WFTE    0x00000040  // ch0_wfifo_thd_en
#define DDRC_WDATTHD0_C0WFT     0x0000003F  // ch0_wfifo_thd

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_WDATTHD1 register.
//
//*****************************************************************************
#define DDRC_WDATTHD1_C6WFTE    0x00400000  // ch6_wfifo_thd_en
#define DDRC_WDATTHD1_C6WFT     0x003F0000  // ch6_wfifo_thd
#define DDRC_WDATTHD1_C5WFTE    0x00004000  // ch5_wfifo_thd_en
#define DDRC_WDATTHD1_C5WFT     0x00003F00  // ch5_wfifo_thd
#define DDRC_WDATTHD1_C4WFTE    0x00000040  // ch4_wfifo_thd_en
#define DDRC_WDATTHD1_C4WFT     0x0000003F  // ch4_wfifo_thd

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_IPORTWPRI register.
// Priority affects on arbitration between ports. Transfers with higher priority
// will be grant first.
// port_pri: priority provided by masters.
// iport_pri: register configurable which applies to each port.
// iport_wpri is write channel priority. iport_rpri is read channel priority
// The finial priority is the larger value of port_pri and iport_pri.
//
//*****************************************************************************
#define DDRC_IPORTWPRI_C6IWPE   0x08000000  // ch6_iport_wpri_en, Enable iport_wpri
                                            // 0: disable, use external priority
                                            // 1: enable, use max priority of external and internal priority
#define DDRC_IPORTWPRI_C6IWP    0x03000000  // ch6_iport_wpri, Priority of write channel 6
                                            // 0x3: the highest priority
                                            // ...
                                            // 0x0: the lowest priority
#define DDRC_IPORTWPRI_C5IWPE   0x00800000  // ch5_iport_wpri_en
#define DDRC_IPORTWPRI_C5IWP    0x00300000  // ch5_iport_wpri
#define DDRC_IPORTWPRI_C4IWPE   0x00080000  // ch4_iport_wpri_en
#define DDRC_IPORTWPRI_C4IWP    0x00030000  // ch4_iport_wpri
#define DDRC_IPORTWPRI_C3IWPE   0x00008000  // ch3_iport_wpri_en
#define DDRC_IPORTWPRI_C3IWP    0x00003000  // ch3_iport_wpri
#define DDRC_IPORTWPRI_C2IWPE   0x00000800  // ch2_iport_wpri_en
#define DDRC_IPORTWPRI_C2IWP    0x00000300  // ch2_iport_wpri
#define DDRC_IPORTWPRI_C1IWPE   0x00000008  // ch1_iport_wpri_en
#define DDRC_IPORTWPRI_C1IWP    0x00000003  // ch1_iport_wpri

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_IPORTRPRI register.
//
//*****************************************************************************
#define DDRC_IPORTRPRI_C6IRPE   0x08000000  //  ch6_iport_rpri_en. Enable iport_rpri
#define DDRC_IPORTRPRI_C6IRP    0x03000000  //  ch6_iport_rpri. Priority of read channel 6
#define DDRC_IPORTRPRI_C5IRPE   0x00800000  //  ch5_iport_rpri_en
#define DDRC_IPORTRPRI_C5IRP    0x00300000  //  ch5_iport_rpri
#define DDRC_IPORTRPRI_C4IRPE   0x00080000  //  ch4_iport_rpri_en
#define DDRC_IPORTRPRI_C4IRP    0x00030000  //  ch4_iport_rpri
#define DDRC_IPORTRPRI_C3IRPE   0x00008000  //  ch3_iport_rpri_en
#define DDRC_IPORTRPRI_C3_IRP   0x00003000  //  ch3_iport_rpri
#define DDRC_IPORTRPRI_C2IRPE   0x00000800  //  ch2_iport_rpri_en
#define DDRC_IPORTRPRI_C2IRP    0x00000300  //  ch2_iport_rpri
#define DDRC_IPORTRPRI_C1IRPE   0x00000008  //  ch1_iport_rpri_en
#define DDRC_IPORTRPRI_C1IRP    0x00000003  //  ch1_iport_rpri

//*****************************************************************************
//
// The following are defines for the bit fields in the DDRC_O_CHxWDOS0 register.
// QoS: the QoS controller is a timer, it indicates that how long the command is
// pending. Once the timer exceed predefined threshold(con), the priority increases
// by 1, up to top-limit(max). The higher priority is, the sooner it will be processed.
//
//*****************************************************************************
#define DDRC_CHxWDOS0_

#endif /* __DDRC_H__ */

