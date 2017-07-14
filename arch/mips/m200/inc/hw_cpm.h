//*****************************************************************************
//
// hw_cpm.h - Macros used when accessing the Clock Power Management hardware.
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

#ifndef __HW_CPM_H__
#define __HW_CPM_H__

//*****************************************************************************
//
// The following are defines for the CPM register offsets.
//
//*****************************************************************************
#define CPM_O_CCR               0x00000000  // Clock Control Register
#define CPM_O_LCR               0x00000004  // Low Power Control Register
#define CPM_O_PCR               0x0000000C  // PLL Control Register
#define CPM_O_APCR              0x00000010  // APLL Control Register
#define CPM_O_MPCR              0x00000014  // MPLL Control Register
#define CPM_O_APACR             0x00000018  // APLL Fraction Register
#define CPM_O_MPACR             0x0000001C  // MPLL Fraction Register
#define CPM_O_CLKGR0            0x00000020  // Clock Gate Register0
#define CPM_O_OPCR              0x00000024  // Oscillator and Power Control Register
#define CPM_O_CLKGR1            0x00000028  // Clock Gate Register1
#define CPM_O_DDRCDR            0x0000002C  // DDR clock divider Register
#define CPM_O_VPUCDR            0x00000030  // VPU clock divider Register
#define CPM_O_SPR               0x00000034  // CPM Scratch Pad Register
#define CPM_O_SPPR              0x00000038  // CPM Scratch Protected Register
#define CPM_O_USBPCR0           0x0000003C  // USB Parameter control register 0
#define CPM_O_USBRDT            0x00000040  // USB Reset Detect Timer Register
#define CPM_O_USBVBFIL          0x00000044  // USB jitter filter Register
#define CPM_O_USBPCR1           0x00000048  // USB Parameter control register 1
#define CPM_O_USBCDR            0x00000050  // OTG PHY clock divider Register
#define CPM_O_I2SCDR            0x00000060  // I2S device clock divider Register
#define CPM_O_LCDPCDR           0x00000064  // LCD Pixel clock divider Register
#define CPM_O_MSC0CDR           0x00000068  // MSC0 clock divider Register
#define CPM_O_UHCCDR            0x0000006C  // UHC 48M clock divider Register
#define CPM_O_SSICDR            0x00000074  // SSI clock divider Register
#define CPM_O_CIMCDR            0x0000007C  // CIM MCLK clock divider Register
#define CPM_O_ISPCDR            0x00000080  // HDMI clock divider Register
#define CPM_O_PCMCDR            0x00000084  // PCM device clock divider Register
#define CPM_O_GPUCDR            0x00000088  // GPU clock divider Register
#define CPM_O_PSWC0ST           0x00000090  // Power Switch Chain0 Start Time
#define CPM_O_PSWC1ST           0x00000094  // Power Switch Chain1 Start Time
#define CPM_O_PSWC2ST           0x00000098  // Power Switch Chain2 Start Time
#define CPM_O_PSWC3ST           0x0000009C  // Power Switch Chain3 Start Time
#define CPM_O_MSC1CDR           0x000000A4  // MSC1 clock divider Register
#define CPM_O_MSC2CDR           0x000000A8  // MSC2 clock divider Register
#define CPM_O_BCHCDR            0x000000AC  // BCH clock divider Register
#define CPM_O_INTR              0x000000B0  // CPM interrupt Register
#define CPM_O_INTRE             0x000000B4  // CPM interrupt Enable Register
#define CPM_O_SPCR0             0x000000B8  // SRAM Power Control Register0
#define CPM_O_SRBC              0x000000C4  // Soft Reset and Bus Control Register
#define CPM_O_SLBC              0x000000C8  // Sleep Boot Control Register
#define CPM_O_SLPC              0x000000CC  // Sleep PC Register
#define CPM_O_DDRCGR            0x000000D0  // DDR clock gate register
#define CPM_O_CSR               0x000000D4  // Clock status register
#define CPM_O_ERNG              0x000000D8  // Enable rng regsiter
#define CPM_O_RNG               0x000000DC  // rng regsiter
#define CPM_O_PGR               0x000000E4  // Power Gate Register

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_CCR register.
//
//*****************************************************************************
#define CPM_CCR_SELSRC          0xC0000000  // Select the clock source for SCLK_A(the MUX clock output)
                                            // 00: stop MUX clock output
                                            // 01: APLL
                                            // 10: EXTCLK
#define CPM_CCR_SELCPLL         0x30000000  // Select the clock source for CPU and L2Cache
                                            // 00: stop MUX clock output
                                            // 01: SCLK_A
                                            // 10: MPLL
#define CPM_CCR_SELH0PLL        0x0C000000  // Select the clock source for AHB0 bus
                                            // 00: stop MUX clock output
                                            // 01: SCLK_A
                                            // 10: MPLL
#define CPM_CCR_SELH2PLL        0x03000000  // Select the clock source for AHB2 bus
                                            // 00: stop MUX clock output
                                            // 01: SCLK_A
                                            // 10: MPLL
#define CPM_CCR_GATESCLKA       0x00800000  // 0: SCLK_A not gating
                                            // 1: SCLK_A is gating
                                            // Gating module: LCD GPU VPU ISP MSC BCH AIC USB UHC SSI PCM CIM_MCLK etc
#define CPM_CCR_CECPU           0x00400000  // 0: Can't write CDIV and L2CDIV
                                            // 1: Could write CDIV and L2CDIV
#define CPM_CCR_CEAHB0          0x00200000  // 0: Can't write H0DIV
                                            // 1: Could write H0DIV
#define CPM_CCR_CEAHB2          0x00100000  // 0: Can't write H2DIV and PDIV
                                            // 1: Could write H2DIV and PDIV
#define CPM_CCR_PDIV            0x000F0000  // Divider for Peripheral Frequency. Specified the PCLK division ratio
                                            // 1/1 ~ 1/14, PDIV=15 and CEAHB2=1 will stop PCLK
                                            //             PDIV=14 and CEAHB2=1 PCLK will continue run(exit stop status)
#define CPM_CCR_H2DIV           0x0000F000  // Divider for AHB2 Frequency. Specified the AHB2 CLK division ratio
                                            // 1/1 ~ 1/14, H2DIV=15 and CEAHB2=1 will stop H2CLK
                                            //             H2DIC=14 and CEAHB2=1 H2CLK will continue run(exit stop status)
#define CPM_CCR_H0DIV           0x00000F00  // Divider for AHB0 Frequency. Specified the AHB0 CLK division ratio
                                            // 1/1 ~ 1/14, H0DIV=15 and CEAHB0=1 will stop H0CLK
                                            //             H0DIC=14 and CEAHB0=1 H0CLK will continue run(exit stop status)
#define CPM_CCR_L2CDIV          0x000000F0  // Divider for L2CACHE Frequency. Specified the L2CLK division ratio
                                            // 1/1 ~ 1/14, L2CDIV=15 and CECPU=1 will stop L2CLK
                                            //             L2CDIC=14 and CECPU=1 L2CLK will continue run(exit stop status)
#define CPM_CCR_CDIV            0x0000000F  // Divider for CPU Frequency. Specified the CPU CLK division ratio
                                            // 1/1 ~ 1/14, CDIV=15 and CECPU=1 will stop CPU CLK
                                            //             CDIC=14 and CECPU=1 CPU CLK will continue run(exit stop status)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_LCR register.
//
//*****************************************************************************
#define CPM_LCR_PDP0            0x80000000  // Power Down big core CPU
                                            // 0: not shut down power supply to big core CPU
                                            // 1: shut down power supply to big core CPU
#define CPM_LCR_PDP1            0x40000000  // Power Down small core CPU
#define CPM_LCR_PDVPU           0x20000000  // Power Down Module VPU
#define CPM_LCR_PDGPU           0x10000000  // Power Down Module GPU
#define CPM_LCR_PDISP           0x08000000  // Power Down Module ISP
#define CPM_LCR_PDH2D           0x04000000  // Power Down Module IPU
#define CPM_LCR_P0S             0x02000000  // Big core CPU power down status.
                                            // 0: Big core CPU module not shut down
                                            // 1: Big core CPU module has entered shut down mode
#define CPM_LCR_P1S             0x01000000  // Small core CPU power down status
#define CPM_LCR_VPUS            0x00800000  // VPU power down status
#define CPM_LCR_GPUS            0x00400000  // GPU power down status
#define CPM_LCR_ISPS            0x00200000  // ISP power down status
#define CPM_LCR_H2DS            0x00100000  // H2D power down status
#define CPM_LCR_PST             0x000FFF00  // Power stability Time. Specifies the Power stabilize time by unit of
                                            // RTCCLK (approximate 32kHz) cycles
#define CPM_LCR_PDDMIC          0x00000080  // Power Dwon DMIC
#define CPM_LCR_DMICS           0x00000040  // DMIC pwer down status
#define CPM_LCR_IDLEDS          0x00000008  // When enter idle mode, set IDLE_DS to 1, the cache and p1`s RAM
                                            // enters rentention mode, software should set OPRCS.mask_int to 1
#define CPM_LCR_SLEEPDS         0x00000004  // When enter sleep mode, set SLEEP_DS 1, only OPCR.pd bit = 00,
                                            // Small core cpu and L2cache RAM enters retention mode. This is only don't shut down P0o or P1
#define CPM_LCR_LPM             0x00000003  // Low Power Mode. Specifies which low-power mode will be entered when SLEEP instruction is executed
                                            // 00: IDLE mode will be entered when SLEEP instruction is executed
                                            // 01: SLEEP mode will be entered when SLEEP instruction is executed

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PCR register.
//
//*****************************************************************************
#define CPM_PCR_ADACPD          0x80000000  // APLL DAC Power Down
                                            // 0: active mode 1: test mode
#define CPM_PCR_APLLBP          0x40000000  // APLL ByPass Mode
                                            // 0: not bypass 1: bypass mode
#define CPM_PCR_MDACPD          0x20000000  // MPLL DAC Power Down
                                            // 0: active mode 1: test mode
#define CPM_PCR_PLLST           0x000000FF  // PLL Stabilize Time. Specifies the PLL stabilize time by unit of RTCCLK
                                            // (approximate 32kHz) cycles. It is used when change PLL multiplier or
                                            // change PLL from off to on. It is initialized to H`20. It is used to control all PLL

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_APCR register.
// Note: VCO(Voltage Controlled Oscillator)
// APLLOD0 is connected to POSTDIV2, APLLOD1 is connected to POSTDIV1. and POSTDIV1 must be
// equal or greater than POSTDIV2. so APLLOD1 must be equal or greater than APLLOD0
//*****************************************************************************
#define CPM_APCR_APLLM          0xFFF00000  // APLL feedback 12-bit multiplication factor, NF = APLLM[11:0]
#define CPM_APCR_APLLN          0x000FC000  // APLL input reference 6-bit divider, NR = APLLN[5:0]
#define CPM_APCR_APLLOD1        0x00003800  // APLL post VCO divider1, OD1 = APLLOD1[2:0]
#define CPM_APCR_APLLOD0        0x00000700  // APLL post VCO divider0, OD0 = APLLOD0[2:0]
#define CPM_APCR_ADSMPD         0x00000080  // Power Down Delta-Sigma Modulator
                                            // 0: DSM is active
                                            // 1: DSM is power down
#define CPM_APCR_APHASEPD       0x00000040  // Power down 4 phase clock generator
                                            // 0: active 1: power down
#define CPM_APCR_APOSTDIVPD     0x00000020  // PLL post divide power down
                                            // 0: active 1: power down
#define CPM_APCR_AVCOPD         0x00000010  // VCO output Power Down
                                            // 0: active 1: power down
#define CPM_APCR_APLLON         0x00000008  // PLL ON Flag
                                            // 0: PLL is off or not stable
                                            // 1: PLL is on and stable
#define CPM_APCR_ALOCK          0x00000004  //
#define CPM_APCR_APLLEN         0x00000001  // APLL Enable. When APLLEN is set to 1, APLL starts to lock phase.
                                            // After APLL stabilizes, APLL_ON bit is set

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_MPCR register.
// Note: MPLLOD0 is connected to POSTDIV2, MPLLOD1 is connected to POSTDIV1. and POSTDIV1 must be
// equal or greater than POSTDIV2. so MPLLOD1 must be equal or greater than MPLLOD0
//
//*****************************************************************************
#define CPM_MPCR_MPLLM          0xFFF00000  // MPLL feedback 12-bit multiplication factor, NF = MPLLM[11:0]
#define CPM_MPCR_MPLLN          0x000FC000  // MPLL input reference 6-bit divider, NR = MPLLN[5:0]
#define CPM_MPCR_MPLLOD1        0x00003800  // MPLL post VCO divider1, OD1 = MPLLOD1[2:0]
#define CPM_MPCR_MPLLOD0        0x00000700  // MPLL post VCO divider0, OD0 = MPLLOD0[2:0]
#define CPM_MPCR_MDSMPD         0x00000080  // Power Down Delta-Sigma Modulator(DSM)
                                            // 0: DSM is active
                                            // 1: DSM is power down
#define CPM_MPCR_MPHASEPD       0x00000040  // Power down 4 phase clock generator
                                            // 0: active 1: power down
#define CPM_MPCR_MPOSTDIVPD     0x00000020  // PLL post divide power down
                                            // 0: active 1: power down
#define CPM_MPCR_MVCOPD         0x00000010  // VCO output Power Down
                                            // 0: active 1: power down
#define CPM_MPCR_MPLLON         0x00000008  // PLL ON Flag.
                                            // 0: PLL is off or not stable
                                            // 1: PLL is on and stable
#define CPM_MPCR_MLOCK          0x00000004  //
#define CPM_MPCR_MPLLEN         0x00000001  // MPLL Enable. When MPLLEN is set to 1, MPLL starts to lock phase.
                                            // After MPLL stabilizes, MPLL_ON bit is set

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_APACR register.
//
//*****************************************************************************
#define CPM_APACR_FRAC          0x00FFFFFF  // Fractional part of the APLL feedback divide value controlled by the
                                            // Delta-Sigma modulator. Total feedback divide value is (FBDIV + FRAC/2^24)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_MPACR register.
//
//*****************************************************************************
#define CPM_MPACR_FRAC          0x00FFFFFF  // Fractional part of the MPLL feedback divide value controlled by the
                                            // Delta-Sigma modulator. Total feedback divide value is (FBDIV + FRAC/2^24)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_CLKGR0 register.
//
//*****************************************************************************
#define CPM_CLKGR0_DDR          0x80000000  // Gate DDR's clock, 0: clock supply 1: stop supply
#define CPM_CLKGR0_TCU          0x40000000  // Gate TCU's pclk
#define CPM_CLKGR0_RTC          0x20000000  // Gate pclk of RTC on apb
#define CPM_CLKGR0_DES          0x10000000  // Gate DES's clock
#define CPM_CLKGR0_PCM          0x08000000  // Gate PCM's clock
#define CPM_CLKGR0_DSI          0x04000000  // Gate DSI's clock
#define CPM_CLKGR0_CSI          0x02000000  // Gate CSI's clock
#define CPM_CLKGR0_LCD          0x01000000  // Gate LCD's clock
#define CPM_CLKGR0_ISP          0x00800000  // Gate ISP's clock
#define CPM_CLKGR0_UHC          0x00400000  // Gate UHC's clock
#define CPM_CLKGR0_PDMA         0x00200000  // Gate PDMA's clock
#define CPM_CLKGR0_SSI2         0x00100000  // Gate SSI2's clock
#define CPM_CLKGR0_SSI1         0x00080000  // Gate SSI1's clock
#define CPM_CLKGR0_UART4        0x00040000  // Gate UART4's clock
#define CPM_CLKGR0_UART3        0x00020000  // Gate UART3's clock
#define CPM_CLKGR0_UART2        0x00010000  // Gate UART2's clock
#define CPM_CLKGR0_UART1        0x00008000  // Gate UART1's clock
#define CPM_CLKGR0_UART0        0x00004000  // Gate UART0's clock
#define CPM_CLKGR0_SADC         0x00002000  // Gate SADC's clock
#define CPM_CLKGR0_MSC2         0x00001000  // Gate MSC2's clock
#define CPM_CLKGR0_AIC          0x00000800  // Gate AIC's clock
#define CPM_CLKGR0_SMB3         0x00000400  // Gate SMB3's clock
#define CPM_CLKGR0_SMB2         0x00000200  // Gate SMB2's clock
#define CPM_CLKGR0_SMB1         0x00000100  // Gate SMB1's clock
#define CPM_CLKGR0_SMB0         0x00000080  // Gate SMB0's clock
#define CPM_CLKGR0_SSI0         0x00000040  // Gate SSI0's clock
#define CPM_CLKGR0_MSC1         0x00000020  // Gate MSC1's clock
#define CPM_CLKGR0_MSC0         0x00000010  // Gate MSC0's clock
#define CPM_CLKGR0_OTG          0x00000008  // Gate OTG's clock
#define CPM_CLKGR0_BCH          0x00000004  // Gate BCH's clock
#define CPM_CLKGR0_NEMC         0x00000002  // Gate NEMC's clock
#define CPM_CLKGR0_NFI          0x00000001  // Gate NFI's clock

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_OPCR register.
//
//*****************************************************************************
#define CPM_OPCR_IDLEDIS        0x80000000  // 0: when CPU enters idle mode, CPU clock is stopped
                                            // 1: When CPU enters idle mode, CPU clock is not stopped
#define CPM_OPCR_MASKINT        0x40000000  // 0: interrupt not mask
                                            // 1: interrupt masked, When enters sleep/idle mode, cpu should set MASK_INT 1, hardware will clear.
#define CPM_OPCR_MASKVPU        0x20000000  // Don`t change this bit , it is only used to debug
#define CPM_OPCR_GPUACTIVE      0x10000000  // GPU active status
#define CPM_OPCR_L2CPD          0x08000000  // 0: L2MP doesn't power down
                                            // 1: L2MP power down when CPU enters sleep mode
#define CPM_OPCR_PD             0x06000000  // 00: CPU doesn't power down
                                            // 01: big core power down
                                            // 10: small core power down
                                            // 11: doesn't power down
#define CPM_OPCR_REQMODE        0x01000000  // 0: Before go into sleep mode, CPM send stop req signal to DDR
                                            // 1: Before go into sleep mode, CPM doesn't send stop req signal to DDR
                                            // When DDR enters retention mode, software should be set the bit to 1
#define CPM_OPCR_RTCLKEN        0x00800000  // 0: RTCLK isn't enable
                                            // 1: RTCLK enable
                                            // This bit control the rtclk to ssi0,ssi1,ssi2,sadc and usb
#define CPM_OPCR_O1ST           0x000FFF00  // EXCLK Oscillator Stabilize Time. This filed specifies the EXCLK oscillator stabilize time by unit of 16 RTCCLK periods
                                            // (oscillator stable time O1ST # 16 / 32768) cycles. It is initialized to H'15
#define CPM_OPCR_SPENDN0        0x00000080  // Force port0(otg) to enter suspend mode
                                            // 0: port0(otg) has forced to entered SUSPEND mode
                                            // 1: port0(otg) hasn`t forced to entered SUSPEND mode
#define CPM_OPCR_SPENDN1        0x00000040  // Force port1(uhc) to enter suspend mode
#define CPM_OPCR_O1SE           0x00000010  // EXCLK Oscillator Sleep Mode Enable. This filed controls the state of the EXCLK oscillator in Sleep mode
                                            // 0: EXCLK oscillator is disabled in Sleep mode
                                            // 1: EXCLK oscillator is enabled in Sleep mode
#define CPM_OPCR_ERCS           0x00000004  // EXCLK/512 clock and RTCLK clock selection
                                            // 0: select EXCLK/512 division ration clock
                                            // 1: select RTCLK clock
                                            // the clock only output to CPM INTC SSI TCU etc
#define CPM_OPCR_BUSMODE        0x00000002  // The default value is 0 When the value is 1, the BUS access is acclerated

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_CLKGR1 register.
//
//*****************************************************************************
#define CPM_CLKGR1_CPU          0x00008000  // This does not gate cpu clock
                                            // 0: cpm will give stp req to CPU
                                            // 1: cpm will not give stp req to CPU

#define CPM_CLKGR1_APB0         0x00004000  // This will gate p0clk of some modules on ahb0 whose slave port is apb. such ad ddr, csi and dsi
#define CPM_CLKGR1_DLINE        0x00002000  // Gate for delay_line's hclk
#define CPM_CLKGR1_TCUEXCLK     0x00001000  // Gate for TCU's EXCLK
#define CPM_CLKGR1_SYSOST       0x00000800  // Gate for sys_ost's hclk
#define CPM_CLKGR1_AHB0         0x00000400  // Gate for AHB0
#define CPM_CLKGR1_P0           0x00000200  // Gate for big cpu core
#define CPM_CLKGR1_P1           0x00000100  // Gate for small cpu core
#define CPM_CLKGR1_DMIC         0x00000080  // Gate for DMIC
                                            // 0: support clock for DMIC
                                            // 1: no clock for DMIC
#define CPM_CLKGR1_HASH         0x00000040  // Gate for HASH
#define CPM_CLKGR1_AES          0x00000020  // Gate for AES
#define CPM_CLKGR1_EPD          0x00000010  // Gate for EPD
#define CPM_CLKGR1_AHBMON       0x00000008  // TODO
#define CPM_CLKGR1_IPU          0x00000004  // Gate for IPU
#define CPM_CLKGR1_GPU          0x00000002  // Gate for GPU
#define CPM_CLKGR1_VPU          0x00000001  // Gate for VPU

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_DDRCDR register.
//
//*****************************************************************************
#define CPM_DDRCDR_DCS          0xC0000000  // DDR Clock Source Selection. Selects the DDR clock source between APLL output and MPLL output.
                                            // 00: stop MUX clock output
                                            // 01: DDR clock source is SCLK_A
                                            // 10: DDR clock source is MPLL
#define CPM_DDRCDR_CEDDR        0x20000000  // Change enable for DDR. If CE_DDR is 1 , write on DDRCDR will start a
                                            // frequency changing sequence immediately. If CE_DDR is 0, writes on has no effect
#define CPM_DDRCDR_DDRBUSY      0x10000000  // The bit is ready only bit. It indicates whether the frequency change has finished.
                                            // 0: indicates frequency change has finished
                                            // 1: indicates the frequency change is on going
#define CPM_DDRCDR_DDRSTOP      0x08000000  // When DDR_STOP is 1 and CE_DDR is 1, the DDR clock will stop
                                            // When DDR_STOP is 0 and CE_DDR is 1, the DDR clock will continue
                                            // DDR_STOP is prior to DDRCDR
#define CPM_DDRCDR_GATEEN       0x04000000  // 1: if ddr gives a clk stop request, cpm will gate ddr`s clk.
#define CPM_DDRCDR_CHANGEN      0x02000000  // When this bit set high ,cpm will change ddr`s clk when when CE_DDR is high and FLAG is low
#define CPM_DDRCDR_FLAG         0x01000000  // Will be set high when ddr gives relative flag, Can only be write zero by software
#define CPM_DDRCDR_DDRCDR       0x0000000F  // Divider for DDR Frequency. Specified the DDR memory clock division ratio, which varies from 1 to 16 (division ratio = DDRCDR + 1)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_VPUCDR register.
//
//*****************************************************************************
#define CPM_VPUCDR_VCS          0x80000000  // VPU Clock Source Selection. Selects the VPU clock source
                                            // 0: VPU clock source is SCLK_A
                                            // 1: VPU clock source is MPLL
#define CPM_VPUCDR_CEVPU        0x20000000  // Change enable for VPU. If CE_VPU is 1 , write on VPUCDR will start a
                                            // frequency changing sequence immediately. If CE_VPU is 0, writes on VPUCDR have no affect
#define CPM_VPUCDR_VPUBUSY      0x10000000  // 0: indicates frequency change has finished
                                            // 1: indicates the frequency change is on going
#define CPM_VPUCDR_VPUSTOP      0x08000000  // When VPU_STOP is 1 and CE_VPU is 1, the VPU clock will stop
                                            // When VPU_STOP is 0 and CE_VPU is 1, the VPU clock will continue
#define CPM_VPUCDR_VPUCDR       0x0000000F  // Divider for VPU Frequency. Specified the VPU clock division ratio, which varies from 1 to 16 (division ratio = VPUCDR + 1)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SPR register.
//
//*****************************************************************************
#define CPM_SPR_CPPSR           0xFFFFFFFF  // allows software to preserve some critical data

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SPPR register.
//
//*****************************************************************************
#define CPM_SPPR_CPSPPR         0x0000FFFF  // The value is only = 0x00005a5a, software can write the CPSPR

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_USBPCR0 register.
// Note: port1 is uhc, port 0 is otg
//*****************************************************************************
#define CPM_USBPCR0_USBMODE     0x80000000  // 0: work as USB device
                                            // 1: work as OTG
#define CPM_USBPCR0_AVLDREG     0x40000000  // This bit is used to set "avalid"(VBUS above A-device session threshold) signal
#define CPM_USBPCR0_IDPULLUPMASK 0x30000000 // These 2 bits control -idpullup# signal in otg mode
                                            // 00: use "idpullup" from otg controller
                                            // 01: "idpullup" always active when usb suspend
                                            // 10: "idpullup" always active
                                            // 11: "idpullup" always active
#define CPM_USBPCR0_INCRMASK    0x08000000  // This bit controls whether the ahb interface enhancement for
                                            // "incr transfer" takes effect. Set this bit to 0 will active the enhancement
#define CPM_USBPCR0_TXRISETUNE  0x04000000  // This bit adjusts the rise/fall times of the high-speed waveform
                                            // 0: default
                                            // 1: -8%
#define CPM_USBPCR0_COMMONONN   0x02000000  // This bit is the USB PHY common block power down control signal
                                            // 0: The common blocks remain powered in suspend mode
                                            // 1: The common blocks are powered down in suspend mode
#define CPM_USBPCR0_VBUSVLDEXT  0x01000000  // External VBUS Valid Indicator. This signal is valid in Device mode and only when the
                                            // VBUSVLDEXTSEL signal is set to 1`b1. VBUSVLDEXT indicates whether the VBUS signal on the USB cable is valid.
                                            // In addition, VBUSVLDEXT enables the pull-up resistor on the
                                            // D+ line. VBUSVLDEXT does not change the OTGSESSVLD0 output.
                                            // 0: The VBUS signal is not valid, and the pull-up resistor on D+ is disabled.
                                            // 1: The VBUS signal is valid, and the pull-up resistor on D+ is enabled
                                            // In Host mode, this input is not used.
#define CPM_USBPCR0_VBUSVLDEXTSEL 0x00800000 // External VBUS Valid Select. This signal selects either the VBUSVLDEXT input or the
                                            // internal Session Valid comparator to detect whether the VBUS signal on the USB cable is valid and to assert the DP pull-up resistor.
                                            // To avoid potential glitches in DP , VBUSVLDEXTSEL must be
                                            // static prior to a power-on reset and remain static during normal operation
                                            // 0: The internal Session Valid comparator is used to assert the DP pull-up resistor
                                            // 1: The VBUSVLDEXT input is used to assert the DP pull-up resistor
#define CPM_USBPCR0_POR         0x00400000  // This signal resets all test registers and state machines in the USB PHY
                                            // The POR signal must be asserted for a minimum of 10 µs.
                                            // 0: not reset 1: reset
                                            // Please make sure SUSPENDM = 1'b1, REFCLKDIV, REFCLKSEL and other USB PHY input signals value are correct before assert POR.
                                            // The PHYCLOCK will available after POR deasserted 45us
#define CPM_USBPCR0_SIDDQ       0x00200000  // This bit is the USB PHY analog blocks power down signal
                                            // Please make this bit to 0
#define CPM_USBPCR0_OTGDISABLE  0x00100000  // This bit is the power control for otg block in USB PHY
                                            // It can powers down the VBUS Valid comparator, but not the
                                            // Session Valid comparator,, nor the ID detection circuitry. To save power, if the application does not use the OTG function,
                                            // this input can be set high
                                            // 0: The OTG block is powered up
                                            // 1: The OTG block is powered down
#define CPM_USBPCR0_COMPDISTUNE 0x000E0000  // These bits control disconnect threshold adjustment.
                                            // 000: -6%
                                            // 001: -4.5%
                                            // 010: -3%
                                            // 011: -1.5%
                                            // 100: Default(0%)
                                            // 101: +1.5%
                                            // 110: +3%
                                            // 111: +4.5%
#define CPM_USBPCR0_OTGTUNE     0x0001C000  // These bits control VBUS valid threshold adjustment
                                            // 000: -6%
                                            // 001: -4.5%
                                            // 010: -3%
                                            // 011: -1.5%
                                            // 100: Default(0%)
                                            // 101: +1.5%
                                            // 110: +3%
                                            // 111: +4.5%
#define CPM_USBPCR0_SQRXTUNE    0x00003800  // These bits control squelch threshold adjustment
                                            // 000: +15%
                                            // 001: +10%
                                            // 010: +5%
                                            // 011: default
                                            // 100: -5%
                                            // 101: -10%
                                            // 110: -15%
                                            // 111: -20%
#define CPM_USBPCR0_TXFSLSTUNE  0x00000780  // These bits control FS/LS source impedance adjustment
                                            // 0000: +5%
                                            // 0001: +2.5%
                                            // 0011: Default
                                            // 0111: -2.5%
                                            // 1111: -5%
#define CPM_USBPCR0_TXPREEMPHTUNE 0x00000040// This bit controls HS transmitter Pre-emphasis enable
                                            // 0: disable 1: enable
#define CPM_USBPCR0_TXHSXVTUNE  0x00000030  // These bits adjust the voltage at which dp and dm signals cross while transmitting in HS mode.
                                            // 00: reserved
                                            // 01: -15mv
                                            // 10: +15mv
                                            // 11: Default
#define CPM_USBPCR0_TXVREFTUNE  0x0000000F  // These bits control HS DC voltage level adjustment
                                            // 0000: -6.25%
                                            // 0001: -5%
                                            // 0010: -3.75%
                                            // 0011: -2.5%
                                            // 0100: -1.25%
                                            // 0101: Default
                                            // 0110: +1.25%
                                            // 0111: +2.5%
                                            // 1000: +3.75%
                                            // 1001: +5%
                                            // 1010: +6.255%
                                            // 1011: +7.5%
                                            // 1100: +8.75%
                                            // 1101: +10%
                                            // 1110: +11.25%
                                            // 1111: +12.5%

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_USBRDT register.
//
//*****************************************************************************
#define CPM_USBRDT_HBMASK       0x04000000  // Halfword/Byte transfer support mask
                                            // 0: enable 1: mask
#define CPM_USBRDT_VBFILLDEN    0x02000000  // VBUS filter data load enable
#define CPM_USBRDT_IDDIGEN      0x01000000  // This bit indicates using CPM_USBRDT_IDDIGREG to control "iddig" signal.
#define CPM_USBRDT_IDDIGREG     0x00800000  // This bit controls "iddig" when CPM_USBRDT_IDDIGEN = 1'b1.
#define CPM_USBRDT_USBRDT       0x007FFFFF  // These bits control USB reset detect time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_USBVBFIL register.
//
//*****************************************************************************
#define CPM_USBVBFIL_IDDIGFIL   0xFFFF0000  // These bits controls iddig jitter filter time
#define CPM_USBVBFIL_USBVBFIL   0x0000FFFF  // These bits controls VBUS jitter filter time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_USBPCR1 register.
// Note: port1 is uhc, port 0 is otg
//*****************************************************************************
#define CPM_USBPCR1_BVLDREG     0x80000000  // This bit is used to set "bvalid"(VBUS above B-device session threshold) signal
#define CPM_USBPCR1_REFCLKSEL   0x0C000000  // Reference Clock Select for USB PHY
                                            // 00: The XO block uses the clock from a crystal.
                                            // 01: The XO block uses an external, 2.5-V clock supplied on the XO pin.
                                            // 10: The PLL uses CLKCORE as reference.
                                            // 11: The PLL uses CLKCORE as reference.
                                            // This bus is a strapping option that must be set prior to a power-on reset
                                            // and remain static during normal operation. Please fix it to 2'b11 or 2'b10
#define CPM_USBPCR1_REFCLKDIV   0x03000000  // This bus selects the reference clock frequency
                                            // 00: 12MHz
                                            // 01: 24MHz
                                            // 10: 48MHz
                                            // 11: 19.2MHz
#define CPM_USBPCR1_DMPD1       0x00800000  // 0: The pull-down resistance on D- of port1 is disabled
                                            // 1: The pull-down resistance on D- of port1 is enabled
#define CPM_USBPCR1_DPPD1       0x00400000  // 0: The pull-down resistance on D+ of port1 is disabled
                                            // 1: The pull-down resistance on D+ of port1 is enabled
#define CPM_USBPCR1_PORT0RST    0x00200000  // Port0 reset
                                            // 0: The transmit and receive finite machines are operational, and the linestate inputs becomes sequential after 11 PHYCLOCK cycles
                                            // 1: The transmit and receive finite state machines are reset, and the linestate logic combinatorially reflects the state of the single-ended receivers
#define CPM_USBPCR1_PORT1RST    0x00100000  // 0: The transmit and receive finite machines are operational, and the linestate inputs becomes sequential after 11 PHYCLOCK cycles
                                            // 1: The transmit and receive finite state machines are reset, and the linestate logic combinatorially reflects the state of the single-ended receivers
#define CPM_USBPCR1_WORDIF0     0x00080000  // This bit selects utmi data bus width of port0
                                            // 0: 8bit/60M
                                            // 1: 16bit/30M
#define CPM_USBPCR1_COMPDISTUNE1 0x00038000 // These bits control disconnect threshold adjustment
                                            // 000: -6%
                                            // 001: -4.5%
                                            // 010: -3%
                                            // 011: -1.5%
                                            // 100: Default
                                            // 101: +1.5%
                                            // 110: +3%
                                            // 111: +4.5%
#define CPM_USBPCR1_SQRXTUNE1   0x00007000  // These bits control squelch threshold adjustment.
                                            // 000: +15%
                                            // 001: +10%
                                            // 010: +5%
                                            // 011: default
                                            // 100: -5%
                                            // 101: -10%
                                            // 110: -15%
                                            // 111: -20%
#define CPM_USBPCR1_TXFSLSTUNE1 0x00000F00  // These bits control FS/LS source impedance adjustment.
                                            // 0000: +5%
                                            // 0001: +2.5%
                                            // 0011: Default
                                            // 0111: -2.5%
                                            // 1111: -5%
#define CPM_USBPCR1_TXPREEMPHTUNE1 0x00000080 // This bit controls HS transmitter Pre-emphasis enable.
                                            // 0: disable 1: enable
#define CPM_USBPCR1_TXHSXVTUNE1 0x00000060  // These bits adjust the voltage at which dp and dm signals cross while transmitting in HS mode
                                            // 00: reserved
                                            // 01: -15mv
                                            // 10: +15mv
                                            // 11: Default
#define CPM_USBPCR1_TXVREFTUNE1 0x0000001E  // These bits control HS DC voltage level adjustment
                                            // 0000: -6.25%
                                            // 0001: -5%
                                            // 0010: -3.75%
                                            // 0011: -2.5%
                                            // 0100: -1.25%
                                            // 0101: Default
                                            // 0110: +1.25%
                                            // 0111: +2.5%
                                            // 1000: +3.75%
                                            // 1001: +5%
                                            // 1010: +6.255%
                                            // 1011: +7.5%
                                            // 1100: +8.75%
                                            // 1101: +10%
                                            // 1110: +11.25%
                                            // 1111: +12.5%
#define CPM_USBPCR1_TXRISETUNE1 0x00000001  // This bit adjusts the rise/fall times of the high-speed waveform
                                            // 0: default
                                            // 1: -8%

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_USBCDR register.
//
//*****************************************************************************
#define CPM_USBCDR_UCS          0x80000000  // USB Clock Source Selection. Selects the USB clock source between PLL output and pin EXCLK
                                            // 0: USB clock source is EXCLK
                                            // 1: USB clock source is PLL output divided by USBCDR
                                            // If UCS == 0, softwar should set USB_STOP bit to reduce power.It is no glitch
                                            // free mux. Software should be stop the module clock, when change this bit
#define CPM_USBCDR_UPCS         0x40000000  // 0: USB clock source is SCLK_A output
                                            // 1: USB clock source is MPLL output
#define CPM_USBCDR_CEUSB        0x20000000  // Change enable for USB. If CE_USB is 1 , write on USBCDR ill start a
                                            // frequency changing sequence immediately. If CE_USB is 0, writes on USBCDR have no affect
#define CPM_USBCDR_USBBUSY      0x10000000  // The bit is read only bit. It indicates whether the frequency change has finished
                                            // 0: indicates frequency change has finished
                                            // 1: indicates the frequency change is on going
#define CPM_USBCDR_USBSTOP      0x08000000  // When USB_STOP is 1 and CE_USB is 1, the USB clock will stop
                                            // When USB_STOP is 0 and CE_USB is 1, the USB clock will continue.
                                            // Software should wait until USB_BUSY == 0, may begin another frequency change
                                            // USB_STOP is prior to USBCDR. and USB_STOP will just stop clk from UPCS, and if UCS is high, usb phy`s CLKCORE will be stopped.
                                            // If system don't use this module, may set this bit to reduce power
#define CPM_USBCDR_USBEN        0x04000000  // 0: USB PHY clock is free runing
                                            // 1: USB PHY clock is shut off
#define CPM_USBCDR_MIPICS       0x02000000  // MIPI clock source selection
                                            // 0: MIPI use exclk
                                            // 1: MIPI use usb's clk
#define CPM_USBCDR_USBCDR       0x000000FF  // Divider for USB Frequency. Specified the USB device clock division ratio
                                            // division ratio = USBCDR + 1

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_I2SCDR register.
//
//*****************************************************************************
#define CPM_I2SCDR_I2SCS        0xC0000000  // 00: SCLK_A output
                                            // 01: MPLL output
                                            // 10: EXCLK
#define CPM_I2SCDR_CEI2S        0x20000000  // Change enable for I2S. If CE_I2S is 1 , write on I2SCDR ill start a
                                            // frequency changing sequence immediately. If CE_I2S is 0, writes on I2SCDR have no affect
#define CPM_I2SCDR_I2SBUSY      0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until I2S_BUSY == 0, may begin another frequency change.
#define CPM_I2SCDR_I2SSTOP      0x08000000  // When I2S_STOP is 1 and CE_I2S is 1, the I2S clock will stop.
                                            // When I2S_STOP is 0 and CE_I2S is 1, the I2S clock will continue.
                                            // Software should wait until I2S_BUSY == 0, may begin another frequency change.
                                            // I2S_STOP is prior to I2SCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_I2SCDR_I2SCDR       0x000000FF  // Divider for I2S Frequency. Specified the I2S device clock division ratio,
                                            // which varies from 1 to 256 (division ratio = I2SCDR + 1)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_LCDPCDR register.
//
//*****************************************************************************
#define CPM_LCDPCDR_LCDPCS      0x80000000  // 0: SCLK_A output
                                            // 1: MPLL output
                                            // It is no glitch free mux. Software should be stop PIX clock, when change this bit.
#define CPM_LCDPCDR_CELCD       0x10000000  // Change enable for LCD. If CE_LCD is 1 , write on LPCDR will start a
                                            // frequency changing sequence immediately. If CE_LCD is 0, writes on LPCDR have no affect
#define CPM_LCDPCDR_LCDBUSY     0x08000000  // The bit is read only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until LCD_BUSY == 0, may begin another frequency change
#define CPM_LCDPCDR_LCDSTOP     0x04000000  // When LCD_STOP is 1 and CE_LCD is 1, the LCD clock will stop.
                                            // When LCD_STOP is 0 and CE_LCD is 1, the LCD clock will continue.
                                            // Software should wait until LCD_BUSY == 0, may begin another frequency change.
                                            // LCD_STOP is prior to LPCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_LCDPCDR_LCDPCDR     0x000000FF  // Divider for Pixel Frequency. Specified the LCD pixel clock (LPCLK)
                                            // division ratio, which varies from 1 to 256 (division ratio = LPCDR + 1)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_MSC0CDR register.
//
//*****************************************************************************
#define CPM_MSC0CDR_MPCS        0x80000000  // 0: select SCLK_A clock output
                                            // 1: select MPLL clock output
                                            // The clock mux is not free of glitches if clock selection is changed
                                            // The clock mux output is to MSC0, MSC1, MSC2 . It is source clock of MSC0, MSC1, MSC2
#define CPM_MSC0CDR_CEMSC0      0x20000000  // Change enable for MSC0. If CE_MSC0 is 1 , write on MSC0CDR ill start a
                                            // frequency changing sequence immediately. If CE_MSC0 is 0, writes on MSC0CDR have no affect
#define CPM_MSC0CDR_MSC0BUSY    0x10000000  // The bit is read only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until MSC0_BUSY == 0, may begin another frequency change
#define CPM_MSC0CDR_MSC0STOP    0x08000000  // When MSC0_STOP is 1 and CE_MSC0 is 1, the MSC0 clock will stop.
                                            // When MSC0_STOP is 0 and CE_MSC0 is 1, the MSC0 clock will continue.
                                            // Software should wait until MSC0_BUSY == 0, may begin another
                                            // frequency change.MSC0_STOP is prior to MSC0CDR. If system don`t use this module, may set this bit to reduce power
#define CPM_MSC0CDR_SCLK0SEL    0x00008000  // MSC0 sample clock selection:
                                            // 0: Sample clock is 90-degree phase shifted by device clock
                                            // 1: Sample clock is 180-degree phase shifted by device clock
#define CPM_MSC0CDR_MSC0CDR     0x000000FF  // Divider for MSC0 Frequency. Specified the MSC0 device clock division
                                            // ratio division ratio = (MSC0CDR + 1)*2

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_UHCCDR register.
//
//*****************************************************************************
#define CPM_UHCCDR_UHCCS        0xC0000000  // 00: UHC clock source is SCLK_A output
                                            // 01: UHC clock source is MPLL output
                                            // 10: UHC clock source is USB_PHY
                                            // It is no glitch free mux. Software should be stop the module clock, when change this bit
                                            // When using USB_PHY, the UHCCDR should be = 0;
#define CPM_UHCCDR_CEUHC        0x20000000  // Change enable for UHC. If CE_UHC is 1 , write on UHCCDR ill start a
                                            // frequency changing sequence immediately. If CE_UHC is 0, writes on UHCCDR have no affect
#define CPM_UHCCDR_UHCBUSY      0x10000000  // The bit is read only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should wait until UHC_BUSY == 0, may begin another frequency change.
#define CPM_UHCCDR_UHCSTOP      0x08000000  // When UHC_STOP is 1 and CE_UHC is 1, the UHC clock will stop.
                                            // When UHC_STOP is 0 and CE_UHC is 1, the UHC clock will continue.
                                            // Software should wait until UHC_BUSY == 0, may begin another frequency change.
                                            // UHC_STOP is prior to UHCCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_UHCCDR_UHCCDR       0x000000FF  // Divider for UHC Frequency. Specified the UHC device clock division ratio
                                            // division ratio = UHCCDR + 1

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SSICDR register.
//
//*****************************************************************************
#define CPM_SSICDR_SSICS        0xC0000000  // 00: select SCLK_A output
                                            // 01: select MPLL output
                                            // 10: EXCLK
                                            // It is no glitch free mux. Software should be stop the module clock, when change this bit
#define CPM_SSICDR_CESSI        0x20000000  // Change enable for SSI. If CE_SSI is 1 , write on SSICDR will start a
                                            // frequency changing sequence immediately. If CE_SSI is 0, writes on SSICDR have no affect
#define CPM_SSICDR_SSIBUSY      0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until SSI_BUSY == 0, may begin another frequency change
#define CPM_SSICDR_SSISTOP      0x08000000  // When SSI_STOP is 1 and CE_SSI is 1, the SSI clock will stop.
                                            // When SSI_STOP is 0 and CE_SSI is 1, the SSI clock will continue.
                                            // Software should wait until SSI_BUSY == 0, may begin another frequency change.
                                            // SSI_STOP is prior to SSICDR. If system don`t use this module, may set this bit to reduce power .
#define CPM_SSICDR_SSICDR       0x000000FF  // Divider for SSI Frequency. Specified the SSI device clock division
                                            // ratio, which varies from 1 to 256 (division ratio = SSICDR + 1).

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_CIMCDR register.
//
//*****************************************************************************
#define CPM_CIMCDR_CIMCS        0x80000000  // 0: select SCLK_A clock output
                                            // 1: select MPLL clock output
                                            // It is no glitch free mux. Software should be stop the module clock, when change this bit
#define CPM_CIMCDR_CECIM        0x40000000  // Change enable for CIM. If CE_CIM is 1 , write on CIMCDR ill start a
                                            // frequency changing sequence immediately. If CE_CIM is 0, writes on CIMCDR have no affect
#define CPM_CIMCDR_CIMBUSY      0x20000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until CIM_BUSY == 0, may begin another frequency change.
#define CPM_CIMCDR_CIMSTOP      0x10000000  // When CIM_STOP is 1 and CE_CIM is 1, the CIM clock will stop.
                                            // When CIM_STOP is 0 and CE_CIM is 1, the CIM clock will continue
                                            // Software should wait until CIM_BUSY == 0, may begin another frequency change.
                                            // CIM_STOP is prior to CIMCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_CIMCDR_CIMCDR       0x000000FF  // Divider for CIM MCLK Frequency. Specified the CIM MCLK clock
                                            // (CIM_MCLK) division ratio, which varies from 1 to 256 (division ratio = CIMCDR + 1). the output clk is io_setup.cim_mclk_o

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_ISPCDR register.
//
//*****************************************************************************
#define CPM_ISPCDR_ISPCS        0x80000000  // 0: select SCLK_A output
                                            // 1: select MPLL output
                                            // It is no glitch free mux. Software should be stop the module clock, when change this bit
#define CPM_ISPCDR_CEISP        0x20000000  // Change enable for ISP. If CE_ISP is 1 , write on ISPCDR ill start a
                                            // frequency changing sequence immediately. If CE_ISP is 0, writes on ISPCDR have no affect
#define CPM_ISPCDR_ISPBUSY      0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until ISP_BUSY == 0, may begin another frequency change
#define CPM_ISPCDR_ISPSTOP      0x08000000  // When ISP_STOP is 1 and CE_ISP is 1, the ISP clock will stop.
                                            // When ISP_STOP is 0 and CE_ISP is 1, the ISP clock will continue.
                                            // Software should wait until ISP_BUSY == 0, may begin another frequency change.
                                            // ISP_STOP is prior to ISPCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_ISPCDR_ISPCDR       0x0000000F  // Divider for ISP Frequency. Specified the ISP clock division ratio, which
                                            // varies from 1 to 16 (division ratio = ISPCDR + 1).

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PCMCDR register.
//
//*****************************************************************************
#define CPM_PCMCDR_PCMCS        0xC0000000  // 00: select SCLK_A output
                                            // 01: select MPLL output
                                            // 10: select EXCLK output
                                            // It is no glitch free mux. Software should be stop the module clock, when change this bit
#define CPM_PCMCDR_CEPCM        0x10000000  // Change enable for PCM. If CE_PCM is 1 , write on PCMCDR ill start a
                                            // frequency changing sequence immediately. If CE_PCM is 0, writes on PCMCDR have no affect
#define CPM_PCMCDR_PCMBUSY      0x08000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until PCM_BUSY == 0, may go next
#define CPM_PCMCDR_PCMSTOP      0x04000000  // When PCM_STOP is 1 and CE_PCM is 1, the PCM clock will stop.
                                            // When PCM_STOP is 0 and CE_PCM is 1, the PCM clock will continue.
                                            // Software should wait until PCM_BUSY == 0, may go next.
                                            // PCM_STOP is prior to PCMCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_PCMCDR_PCMCDR       0x000000FF  // Divider for PCM Frequency. Specified the PCM device clock division ratio,
                                            // which varies from 1 to 256 (division ratio = PCMCDR + 1).

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_GPUCDR register.
//
//*****************************************************************************
#define CPM_GPUCDR_GPUCS        0x80000000  // 0: select SCLK_A output
                                            // 1: select MPLL output
                                            // The clock mux is not free of glitches if clock selection is changed
#define CPM_GPUCDR_CEGPU        0x20000000  // Change enable for GPU. If CE_GPU is 1 , write on GPUCDR ill start a
                                            // frequency changing sequence immediately. If CE_GPU is 0, writes on GPUCDR have no affect
#define CPM_GPUCDR_GPUBUSY      0x10000000  // The bit is read only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until GPU_BUSY == 0, may begin another frequency change
#define CPM_GPUCDR_GPUSTOP      0x08000000  // When GPU_STOP is 1 and CE_GPU is 1, the GPU clock will stop.
                                            // When GPU_STOP is 0 and CE_GPU is 1, the GPU clock will continue.
                                            // Software should wait until GPU_BUSY == 0, may begin another frequency change.
                                            // GPU_STOP is prior to GPUCDR. If system don`t use this module, may set this bit to reduce power
#define CPM_GPUCDR_GPUCDR       0x0000000F  // Divider for GPU Frequency. Specified the GPU clock division ratio, which
                                            // varies from 1 to 16 (division ratio = GPUCDR + 1)

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PSWC0ST register.
// Note: The Start Time by the unit of PCLK cycles
//*****************************************************************************
#define CPM_PSWC0ST_PSWC0ST     0x00000FFF  // Power Switch Chain0 Start Time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PSWC1ST register.
//
//*****************************************************************************
#define CPM_PSWC1ST_PSWC1ST     0x00000FFF  // Power Switch Chain1 Start Time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PSWC2ST register.
//
//*****************************************************************************
#define CPM_PSWC2ST_PSWC2ST     0x00000FFF  // Power Switch Chain2 Start Time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PSWC3ST register.
//
//*****************************************************************************
#define CPM_PSWC3ST_PSWC3ST     0x00000FFF  // Power Switch Chain3 Start Time

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_MSC1CDR register.
//
//*****************************************************************************
#define CPM_MSC1CDR_CEMSC1      0x20000000  // Change enable for MSC1. If CE_MSC1 is 1 , write on MSC1CDR ill start a
                                            // frequency changing sequence immediately. If CE_MSC1 is 0, writes on MSC1CDR have no affect
#define CPM_MSC1CDR_MSC1BUSY    0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until MSC1_BUSY == 0, may go next
#define CPM_MSC1CDR_MSC1STOP    0x08000000  // When MSC1_STOP is 1 and CE_MSC1 is 1, the MSC1 clock will stop.
                                            // When MSC1_STOP is 0 and CE_MSC1 is 1, the MSC1 clock will continue.
                                            // Software should wait until MSC1_BUSY == 0, may begin another
                                            // frequency change.MSC1_STOP is prior to MSC1CDR.
                                            // If system don`t use this module, may set this bit to reduce power
#define CPM_MSC1CDR_SCLK1SEL    0x00008000  // MSC1 sample clock selection:
                                            // 0: Sample clock is 90-degree phase shifted by device clock
                                            // 1: Sample clock is 180-degree phase shifted by device clock
#define CPM_MSC1CDR_MSC1CDR     0x000000FF  // Divider for MSC1 Frequency. Specified the MSC1 device clock division
                                            // ratio division ratio = (MSC1CDR + 1)*2

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_MSC2CDR register.
//
//*****************************************************************************
#define CPM_MSC2CDR_CEMSC2      0x20000000  // Change enable for MSC2. If CE_MSC2 is 1 , write on MSC2CDR ill start a
                                            // frequency changing sequence immediately. If CE_MSC2 is 0, writes on MSC2CDR have no affect
#define CPM_MSC2CDR_MSC2BUSY    0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until MSC2_BUSY == 0, may begin another frequency change
#define CPM_MSC2CDR_MSC2STOP    0x08000000  // When MSC2_STOP is 1 and CE_MSC2 is 1, the MSC2 clock will stop.
                                            // When MSC2_STOP is 0 and CE_MSC2 is 1, the MSC2 clock will continue.
                                            // Software should wait until MSC2_BUSY == 0, may begin another frequency change.
                                            // MSC2_STOP is prior to MSC2CDR. If system don`t use this module, may set this bit to reduce power
#define CPM_MSC2CDR_SCLK2SEL    0x00008000  // MSC2 sample clock selection:
                                            // 0: Sample clock is 90-degree phase shifted by device clock
                                            // 1: Sample clock is 180-degree phase shifted by device clock
#define CPM_MSC2CDR_MSC2CDR     0x000000FF  // Divider for MSC2 Frequency. Specified the MSC2 device clock division
                                            // ratio division ratio = (MSC2CDR + 1)*2.

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_BCHCDR register.
//
//*****************************************************************************
#define CPM_BCHCDR_BCHCS        0xC0000000  // 0: select SCLK_A output
                                            // 1: select MPLL output
                                            // The clock mux isn't free of glitches if clock selection is changed
                                            // When switching clock source, it should be ensured that the clock switched
                                            // from and the clock switched to are running.
#define CPM_BCHCDR_CEBCH        0x20000000  // Change enable for BCH. If CE_BCH is 1 , write on BCHCDR ill start a
                                            // frequency changing sequence immediately. If CE_BCH is 0, writes on BCHCDR have no affect
#define CPM_BCHCDR_BCHBUSY      0x10000000  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until BCH_BUSY == 0, may begin another frequency change
#define CPM_BCHCDR_BCHSTOP      0x08000000  // When BCH_STOP is 1 and CE_BCH is 1, the BCH clock will stop.
                                            // When BCH_STOP is 0 and CE_BCH is 1, the BCH clock will continue.
                                            // Software should wait until BCH_BUSY == 0, may begin another frequency change.
                                            // BCH_STOP is prior to BCHCDR.
                                            // If system don`t use this module, may set this bit to reduce power
#define CPM_BCHCDR_BCHCDR       0x0000000F  // Divider for BCH Frequency. Specified the BCH clock division ratio, which
                                            // varies from 1 to 16 (division ratio = BCHCDR + 1).

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_INTR register.
//
//*****************************************************************************
#define CPM_INTR_VBUSINTR       0x00000002  // B device insert interrupt.
#define CPM_INTR_ADEVINTR       0x00000001  // A device insert interrupt

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_INTRE register.
//
//*****************************************************************************
#define CPM_INTRE_VBUSINTRE     0x00000002  // B device insert interrupt enable
#define CPM_INTRE_ADEVINTRE     0x00000001  // A device insert interrupt enable

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SPCR0 register.
//
//*****************************************************************************
#define CPM_SPCR0_PDMADS        0x80000000  // Deep sleep of pdma
#define CPM_SPCR0_NFI           0x08000000  // Memory Power control for NFI
                                            // 0: on 1: shutdown
#define CPM_SPCR0_AES           0x04000000  // Memory Power control for AES
#define CPM_SPCR0_SSI1          0x02000000  // Memory Power control for SSI1
#define CPM_SPCR0_PCM           0x01000000  // Memory Power control for PCM
#define CPM_SPCR0_SSI2          0x00800000  // Memory Power control for SSI2
#define CPM_SPCR0_I2S           0x00400000  // Memory Power control for I2S
#define CPM_SPCR0_TCU           0x00200000  // Memory Power control for TCU
#define CPM_SPCR0_UART4         0x00100000  // Memory Power control for UART4
#define CPM_SPCR0_UART3         0x00080000  // Memory Power control for UART3
#define CPM_SPCR0_UART2         0x00040000  // Memory Power control for UART2
#define CPM_SPCR0_UART1         0x00020000  // Memory Power control for UART1
#define CPM_SPCR0_UART0         0x00010000  // Memory Power control for UART0
#define CPM_SPCR0_PDMA          0x00008000  // Memory Power control for PDMA
#define CPM_SPCR0_UHC           0x00004000  // Memory Power control for UHC
#define CPM_SPCR0_MSC2          0x00002000  // Memory Power control for MSC2
#define CPM_SPCR0_MSC1          0x00001000  // Memory Power control for MSC1
#define CPM_SPCR0_OTG           0x00000800  // Memory Power control for OTG
#define CPM_SPCR0_LCD           0x00000400  // Memory Power control for LCD
#define CPM_SPCR0_EPD           0x00000200  // Memory Power control for EPD
#define CPM_SPCR0_DSI           0x00000100  // Memory Power control for DSI
#define CPM_SPCR0_DES           0x00000080  // Memory Power control for DES
#define CPM_SPCR0_SADC          0x00000040  // Memory Power control for SADC
#define CPM_SPCR0_SSI0          0x00000020  // Memory Power control for SSI0
#define CPM_SPCR0_MSC0          0x00000010  // Memory Power control for MSC0
#define CPM_SPCR0_BCH           0x00000008  // Memory Power control for BCH
#define CPM_SPCR0_AHB2BRI       0x00000004  // Memory Power control for AHB2_BRI
#define CPM_SPCR0_NEMC          0x00000002  // Memory Power control for NEMC
#define CPM_SPCR0_DDR           0x00000001  // Memory Power control for DDR

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SRBC register.
//
//*****************************************************************************
#define CPM_SRBC_VPUSR          0x80000000  // 0: VPU does not enter soft reset mode
                                            // 1: VPU enters soft reset mode
#define CPM_SRBC_VPUSTP         0x40000000  // Request for VPU to Stop bus transfer
#define CPM_SRBC_VPUACK         0x20000000  // VPU Stop Ack.
#define CPM_SRBC_GPUSR          0x10000000  // 0: GPU does not enter soft reset mode
                                            // 1: GPU enters soft reset mode
#define CPM_SRBC_GPUSTP         0x08000000  // Request for GPU to Stop bus transfer
#define CPM_SRBC_GPUACK         0x04000000  // GPU Stop Ack
#define CPM_SRBC_LCDSR          0x02000000  // 0: LCD does not enter soft reset mode
                                            // 1: LCD enters soft reset mode

#define CPM_SRBC_LCDSTP         0x01000000  // Request for LCD to Stop bus transfer
#define CPM_SRBC_LCDACK         0x00800000  // LCD Stop Ack
#define CPM_SRBC_ISPSR          0x00400000  // 0: ISP does not enter soft reset mode
                                            // 1: ISP enters soft reset mode
#define CPM_SRBC_ISPSTP         0x00200000  // Request for ISP to Stop bus transfer
#define CPM_SRBC_ISPACK         0x00100000  // ISP Stop Ack
#define CPM_SRBC_H2DSR          0x00080000  // 0: H2D does not enter soft reset mode
                                            // 1: H2D enters soft reset mode
#define CPM_SRBC_H2DSTP         0x00040000  // Request for H2D to Stop bus transfer.
#define CPM_SRBC_H2DACK         0x00020000  // H2D Stop Ack
#define CPM_SRBC_CPUSTP         0x00008000  // Request for CPU to Stop bus transfer.
#define CPM_SRBC_CPUACK         0x00004000  // CPU Stop Ack
#define CPM_SRBC_UHCSR          0x00002000  // UHC soft reset
#define CPM_SRBC_OTG1SR         0x00001000  // OTG1 soft reset
#define CPM_SRBC_OTG0SR         0x00000800  // OTG0 soft reset
#define CPM_SRBC_AHB0STP        0x00000400  // Request for AHB0 to Stop bus transfer
#define CPM_SRBC_AHB0ACK        0x00000200  // AHB0 Stop Ack
#define CPM_SRBC_AHB2STP        0x00000100  // Request for AHB2 to Stop bus transfer
#define CPM_SRBC_AHB2ACK        0x00000080  // AHB2 Stop Ack
#define CPM_SRBC_DDRSTP         0x00000040  // Request for DDR to Stop bus transfer
#define CPM_SRBC_DDRACK         0x00000020  // DDR Stop Ack

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SLBC register.
//
//*****************************************************************************
#define CPM_SLBC_SLBC           0x00000001  // 0: Sleep Mode Wake up Boot Process is same with any reset
                                            // 1: Sleep Mode Wake up Boot Process is jumped to SLPC, don`t care Boot_Sel.
                                            // The bit is only valid When shut down P0 in Sleep Mode

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_SLPC register.
//
//*****************************************************************************
#define CPM_SLPC_SLPC           0xFFFFFFFF  // When SLBC is 1, sleep boot is jumped to SLPC, not true boot as any reset.
                                            // The bit is only valid When shut down P0 in Sleep Mode

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_DDRCGR register.
//
//*****************************************************************************
#define CPM_DDRCGR_DRCG         0x000001FF  // Note: Please refer to DDR and NFI`s spec for detail use of this register.

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_CSR register.
//
//*****************************************************************************
#define CPM_CSR_H2DIVBUSY       0x00000004  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until H2DIV_BUSY == 0, then may begin another frequency change
#define CPM_CSR_H0DIVBUSY       0x00000002  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until H0DIV_BUSY == 0, then may begin another frequency change
#define CPM_CSR_CDIVBUSY        0x00000001  // The bit is ready only bit. It indicates whether the frequency change has
                                            // finished. When the bit is 0, it indicates frequency change has finished.,
                                            // otherwise it indicates the frequency change is on going
                                            // Software should be wait until CDIV_BUSY == 0, then may begin another frequency change.

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_ERNG register.
//
//*****************************************************************************
#define CPM_ERNG_READY          0x80000000  // Data ready
                                            // When software read RNG register, hardware auto clear ready to 0
#define CPM_ERNG_ERNG           0x00000001  // 0: not enable 1: enable

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_RNG register.
//
//*****************************************************************************
#define CPM_RNG_RNG             0xFFFFFFFF  // The RNG Register is a 32-bit read only register

//*****************************************************************************
//
// The following are defines for the bit fields in the CPM_O_PGR register.
//
//*****************************************************************************
#define CPM_PGR_BCHS            0x04000000  //
#define CPM_PGR_HASHS           0x02000000  //
#define CPM_PGR_LCDCS           0x00200000  //
#define CPM_PGR_USBS            0x00100000  //
#define CPM_PGR_UHCS            0x00080000  //
#define CPM_PGR_BCH             0x00000400  //
#define CPM_PGR_HASH            0x00000200  //
#define CPM_PGR_EPDC            0x00000040  // EPDC: do not change this bit
#define CPM_PGR_LCDC            0x00000020  //
#define CPM_PGR_USB             0x00000010  //
#define CPM_PGR_UHC             0x00000008  //
#define CPM_PGR_PFMON           0x00000004  //
#define CPM_PGR_CSI             0x00000002  //
#define CPM_PGR_DSI             0x00000001  //

#endif /* __HW_CPM_H__ */
