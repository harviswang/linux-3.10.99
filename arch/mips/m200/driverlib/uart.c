//*****************************************************************************
//
// uart.c - Driver for the UART.
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

#include "../inc/hw_uart.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "debug.h"
#include "uart.h"
#include "intc.h"

static tBoolean UARTDlabSet(unsigned long ulBase);
static tBoolean UARTDlabClear(unsigned long ulBase);

//
// 1s = 1000000000 nanosecond
// using nanosecond to calculate
//
#define ONE_SECOND              1000000000
#define NUMBER                  13
#define ABS(x)                  ((x) >= 0 ? (x) : (-x))

//*****************************************************************************
//
// A mapping of UART base address to interupt number.
//
//*****************************************************************************
static const unsigned long g_ppulUARTIntMap[][2] =
{
    { UART0_BASE, IRQ_NO_UART0 },
    { UART1_BASE, IRQ_NO_UART1 },
    { UART2_BASE, IRQ_NO_UART2 },
    { UART3_BASE, IRQ_NO_UART3 },
    { UART4_BASE, IRQ_NO_UART4 },
};

//*****************************************************************************
//
//! \internal
//! Checks a UART base address.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function determines if a UART port base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
UARTBaseValid(unsigned long ulBase)
{
    return((ulBase == UART0_BASE) || (ulBase == UART1_BASE) ||
           (ulBase == UART2_BASE) || (ulBase == UART3_BASE) ||
           (ulBase == UART4_BASE));
}
#endif

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
UARTRegisterDump(unsigned long ulBase, int (*print)(const char *format, ...))
{
	/*
        //
	// DLAB=0
        //
        UARTDlabClear(ulBase);
	REG_PRINT(UART_O_RBR, ulBase, print);
        //
	// DLAB=0
        //
        UARTDlabClear(ulBase);
	REG_PRINT(UART_O_THR, ulBase, print);
	*/
        //
	// DLAB=1
        //
        UARTDlabSet(ulBase);
	REG_PRINT(UART_O_DLLR, ulBase, print);
        //
	// DLAB=1
        //
        UARTDlabSet(ulBase);
	REG_PRINT(UART_O_DLHR, ulBase, print);
        //
	// DLAB=0
        //
        UARTDlabClear(ulBase);
	REG_PRINT(UART_O_IER, ulBase, print);
	REG_PRINT(UART_O_IIR, ulBase, print);
	// UART_O_FCR is write only, don't read
	//REG_PRINT(UART_O_FCR, ulBase, print);
	REG_PRINT(UART_O_LCR, ulBase, print);
	REG_PRINT(UART_O_MCR, ulBase, print);
	REG_PRINT(UART_O_LSR, ulBase, print);
	REG_PRINT(UART_O_MSR, ulBase, print);
	REG_PRINT(UART_O_SPR, ulBase, print);
	REG_PRINT(UART_O_ISR, ulBase, print);
	REG_PRINT(UART_O_MR, ulBase, print);
	REG_PRINT(UART_O_ACR, ulBase, print);
	REG_PRINT(UART_O_RFCR, ulBase, print);
	REG_PRINT(UART_O_TFCR, ulBase, print);
}

//*****************************************************************************
//
//! \internal
//! Set DLAB
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function set DLAB=1 using to access UART_O_DLLR/UART_O_DLHR.
//!
//! \return Returns \b true if DLAB==1 valid and \b false otherwise
//
//*****************************************************************************
static tBoolean
UARTDlabSet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    HWREG(ulBase + UART_O_LCR) |= UART_LCR_DLAB;

    return((HWREG(ulBase + UART_O_LCR) & UART_LCR_DLAB) == UART_LCR_DLAB);
}

//*****************************************************************************
//
//! \internal
//! Clear DLAB.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function set DLAB=0 using to access UART_O_RBR/UART_O_THR/UART_O_IER.
//!
//! \return Returns \b true if DLAB==0 and \b false otherwise
//
//*****************************************************************************
static tBoolean
UARTDlabClear(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    HWREG(ulBase + UART_O_LCR) &= ~UART_LCR_DLAB;

    return((HWREG(ulBase + UART_O_LCR) & UART_LCR_DLAB) != UART_LCR_DLAB);
}
//*****************************************************************************
//
//! Gets the UART interrupt number.
//!
//! \param ulBase is the base address of the UART port.
//!
//! Given a UART base address, this function returns the corresponding
//! interrupt number.
//!
//! \return Returns a UART interrupt number, or -1 if \e ulBase is invalid.
//
//*****************************************************************************
long
UARTIntNumberGet(unsigned long ulBase)
{
    unsigned long ulIdx;

    //
    // Loop through the table that maps UART base addresses to interrupt
    // numbers.
    //
    for(ulIdx = 0; ulIdx < (sizeof(g_ppulUARTIntMap) /
                            sizeof(g_ppulUARTIntMap[0])); ulIdx++)
    {
        //
        // See if this base address matches.
        //
        if(g_ppulUARTIntMap[ulIdx][0] == ulBase)
        {
            //
            // Return the corresponding interrupt number.
            //
            return(g_ppulUARTIntMap[ulIdx][1]);
        }
    }

    //
    // The base address could not be found, so return an error.
    //
    return(-1);
}

//*****************************************************************************
//
//! Sets the type of parity.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulParity specifies the type of parity to use.
//!
//! This function configures the type of parity to use for transmitting and
//! expect when receiving.  The \e ulParity parameter must be one of
//! \b UART_CONFIG_PAR_NONE, \b UART_CONFIG_PAR_EVEN, \b UART_CONFIG_PAR_ODD,
//! or \b UART_CONFIG_PAR_STICKY.  The last two
//! parameters allow direct control of the parity bit; it is always either one
//! or zero based on the mode.
//!
//! \return None.
//
//*****************************************************************************
void
UARTParityModeSet(unsigned long ulBase, unsigned long ulParity)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    ASSERT((ulParity == UART_CONFIG_PAR_NONE) ||
           (ulParity == UART_CONFIG_PAR_EVEN) ||
           (ulParity == UART_CONFIG_PAR_ODD) ||
           (ulParity == UART_CONFIG_PAR_STICKY));

    //
    // Set the parity mode.
    //
    HWREG(ulBase + UART_O_LCR) = ((HWREG(ulBase + UART_O_LCR) &
                                    ~(UART_LCR_STPAR | UART_LCR_PARM |
                                      UART_LCR_PARE)) | ulParity);
}

//*****************************************************************************
//
//! Gets the type of parity currently being used.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function gets the type of parity used for transmitting data and
//! expected when receiving data.
//!
//! \return Returns the current parity settings, specified as one of
//! \b UART_CONFIG_PAR_NONE, \b UART_CONFIG_PAR_EVEN, \b UART_CONFIG_PAR_ODD,
//! \b UART_CONFIG_PAR_ONE, or \b UART_CONFIG_PAR_ZERO.
//
//*****************************************************************************
unsigned long
UARTParityModeGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Return the current parity setting.
    //
    return(HWREG(ulBase + UART_O_LCR) &
           (UART_LCR_STPAR | UART_LCR_PARM | UART_LCR_PARE));
}

//*****************************************************************************
//
//! Sets the FIFO level at which interrupts are generated.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulRxLevel is the receive FIFO interrupt level, specified as one of
//! \b UART_FIFO_RX1_64, \b UART_FIFO_RX16_64, \b UART_FIFO_RX32_64,
//! \b UART_FIFO_RX60_64
//!
//! This function configures the FIFO level at which transmit and receive
//! interrupts are generated.
//!
//! \return None.
//
//*****************************************************************************
void
UARTFIFOLevelSet(unsigned long ulBase, unsigned long ulRxLevel)
{
    //   
    // Check the arguments.
    //   
    ASSERT(UARTBaseValid(ulBase));
    ASSERT((ulRxLevel == UART_FIFO_RX1_64) ||
           (ulRxLevel == UART_FIFO_RX16_64) ||
           (ulRxLevel == UART_FIFO_RX32_64) ||
           (ulRxLevel == UART_FIFO_RX60_64));

    //   
    // Set the FIFO interrupt levels.
    //   
    HWREG(ulBase + UART_O_FCR) &= ~UART_FIFO_RX_MASK;
    HWREG(ulBase + UART_O_FCR) |= ulRxLevel;
}

//*****************************************************************************
//
//! Gets the FIFO level at which interrupts are generated.
//!
//! \param ulBase is the base address of the UART port.
//! \param pulTxLevel is a pointer to storage for the transmit FIFO level,
//! returned as one of \b UART_FIFO_TX1_8, \b UART_FIFO_TX2_8,
//! \b UART_FIFO_TX3_8, \b UART_FIFO_TX4_8.
//! \param pulRxLevel is a pointer to storage for the receive FIFO level,
//! returned as one of \b UART_FIFO_RX1_8, \b UART_FIFO_RX2_8,
//! \b UART_FIFO_RX3_8, \b UART_FIFO_RX3_8.
//!
//! This function gets the FIFO level at which transmit and receive interrupts
//! are generated.
//!
//! \return None.
//
//*****************************************************************************
void
UARTFIFOLevelGet(unsigned long ulBase, unsigned long *pulTxLevel,
                 unsigned long *pulRxLevel)
{
    unsigned long ulTemp;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Read the FIFO level register.
    //
    ulTemp = HWREG(ulBase + UART_O_FCR);

    //
    // Extract the transmit and receive FIFO levels.
    //
    *pulTxLevel = ulTemp & UART_FCR_RDTR;
    *pulRxLevel = ulTemp & UART_FCR_RDTR;
}

//*****************************************************************************
//
//! Enables the transmit and receive FIFOs.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This functions enables the transmit and receive FIFOs in the UART.
//!
//! \return None.
//
//*****************************************************************************
void
UARTFIFOEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Enable the FIFO.
    //
    HWREG(ulBase + UART_O_FCR) |= UART_FCR_FME;

    //
    // Reset Tx/Rx FIFO
    HWREG(ulBase + UART_O_FCR) |= UART_FCR_TFRT| UART_FCR_RFRT;
}

//*****************************************************************************
//
//! Disables the transmit and receive FIFOs.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function disables the transmit and receive FIFOs in the UART.
//!
//! \return None.
//
//*****************************************************************************
void
UARTFIFODisable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Disable the FIFO.
    //
    HWREG(ulBase + UART_O_FCR) &= ~(UART_FCR_FME);
}

//*****************************************************************************
//
//! Calculate the clock used for generate baud rate signal.
//! \param ulCLKdev is the clock of UART
//! \param ulBaudRate is the desired baud rate
//! \param pulDivisor is a pointer to storage for the divisor, ulCLKdev/ulBaudRate
//!
//! This function get frequency at which to generate desired baud rate
//!
//*****************************************************************************
static unsigned long
BaudRateCLKGet(unsigned long ulCLKdev, unsigned long ulBaudRate, unsigned long *pulDivisor)
{
    unsigned long ulClkbr;
    unsigned long ulDiv;
    
    ulDiv = ulCLKdev / ulBaudRate;
    
    if (pulDivisor) {
        *pulDivisor = ulDiv;
    }
 
    ulClkbr = ulCLKdev / ulDiv;
    return ulClkbr;
}

//*****************************************************************************
//
//! Calculate .
//! \param 
//! \param 
//! \param 
//!
//! This function how to generate more accurate baud rate, by MR and ACR,
//! MR defines how many CLKbr unit to use as an real baud rate period.
//! ACR defines on which bit need to compensate one CLKbr period
//! below is an example, CLKdev=4HMHZ Divisor=1, CLKbr=4MHZ, BaudRate=460800
//! MR = 8, ACR=0b101101101101 12bits
//! 0	2.17	4.34	6.51	8.68	10.85	13.02	15.19	17.36	19.53	21.7	23.87	26.04
//! 0	2.25	4.25	6.5	8.75	10.75	13	15.25	17.25	19.5	21.75	23.75	26
//! 0	0.08	-0.09	-0.01	0.07	-0.1	-0.02	0.06	-0.11	-0.03	0.05	-0.12	-0.04
//!	9	17	26	35	43	52	61	69	78	87	95	104
//!	9	8	9	9	8	9	9	8	9	9	8	9
//!
//! example, CLKdev=24MHz, Divisor=0xd, BaudRate=115200, ulMr=0x10
//! 0	1	2	3	4	5	6	7	8	9	10	11	12	
//! 0	8.68	17.36	26.04	34.72	43.4	52.08	60.76	69.44	78.12	86.8	95.48	104.16	expected timing
//! 0	8.66	17.32	25.98	34.64	43.3	51.96	60.62	69.28	77.94	86.6	95.26	103.92	actual timing
//! 0	0.02	0.04	0.06	0.08	0.1	0.12	0.14	0.16	0.18	0.2	0.22	0.24	delta
//! 0	208 	416	624	832	1040	1248	1456	1664	1872	2080	2288	2496	number of 1/clock
//!	208	208	208	208	208	208	208	208	208	208	208	208	interval

//*****************************************************************************
static unsigned long 
ACR(unsigned long ulCLKbr, unsigned long ulBaudRate, unsigned long *pulMR, 
	            unsigned long *pulErrorSum)
{
	unsigned long ulAcr;
	unsigned long i;
	unsigned long ulTimeExpected[NUMBER] = { 0 };
	unsigned long ulTimeActual[NUMBER] = { 0 };
	         long ulTimeError[NUMBER] = { 0 };
	unsigned long ulTimeCompensate[NUMBER] = { 0 };
	const unsigned long ulNanosecondCLKbr = ONE_SECOND / ulCLKbr;
	const unsigned long ulNanosecondBaudRate = ONE_SECOND / ulBaudRate;
	unsigned long ulMr;
	unsigned long ulErrorSum;

	//
	// Calculate MR
	//
	ulMr = ulCLKbr / ulBaudRate;
	if (pulMR) {
	    *pulMR = ulMr;
	}

	//
	// ulCLKbr % ulBaudRate == 0, no need to compensate, ACR fill to 0
	//
	if (ulCLKbr % ulBaudRate == 0) {
	    return 0;
	}


	//
	// Calculate ulTimeExpected/ulTimeActual/ulTimeError
	//
	ulErrorSum = 0;
	for (i = 0; i < NUMBER; i++) {
	    ulTimeExpected[i] = ulNanosecondBaudRate * i;
	    ulTimeActual[i] = (ulTimeExpected[i] + ulNanosecondCLKbr / 2) 
		    / ulNanosecondCLKbr * ulNanosecondCLKbr;
	    ulTimeError[i] = ulTimeActual[i] - ulTimeExpected[i];
	    ulErrorSum += ABS(ulTimeError[i]);
	}
	
	//
	// Calculate TimeCompensate
	//
	for (i = 1; i < NUMBER; i++) {
	    ulTimeCompensate[i] = (ulTimeActual[i] - ulTimeActual[i - 1])
		    / ulNanosecondCLKbr % ulMr;
	}

	//
	// Calculate ACR register format value, [1, 12] -> [11, 0]
	//
	for (ulAcr = 0, i = 1; i < NUMBER; i++) {
	    if (ulTimeCompensate[i]) {
		ulAcr |= 1 << (11 - (i - 1));
	    }
	}
	
	//
	// Store error sum
	//
	if (pulErrorSum) {
	    *pulErrorSum = ulErrorSum;
	}

	return ulAcr;
}

//*****************************************************************************
//
//! Sets the configuration of a UART.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulUARTClk is the rate of the clock supplied to the UART module.
//! \param ulBaud is the desired baud rate.
//! \param ulConfig is the data format for the port (number of data bits,
//! number of stop bits, and parity).
//!
//! This function configures the UART for operation in the specified data
//! format.  The baud rate is provided in the \e ulBaud parameter and the data
//! format in the \e ulConfig parameter.
//!
//! The \e ulConfig parameter is the logical OR of three values: the number of
//! data bits, the number of stop bits, and the parity.  \b UART_CONFIG_WLEN_8,
//! \b UART_CONFIG_WLEN_7, \b UART_CONFIG_WLEN_6, and \b UART_CONFIG_WLEN_5
//! select from eight to five data bits per byte (respectively).
//! \b UART_CONFIG_STOP_ONE and \b UART_CONFIG_STOP_TWO select one or two stop
//! bits (respectively).  \b UART_CONFIG_PAR_NONE, \b UART_CONFIG_PAR_EVEN,
//! \b UART_CONFIG_PAR_ODD, \b UART_CONFIG_PAR_ONE, and \b UART_CONFIG_PAR_ZERO
//! select the parity mode (no parity bit, even parity bit, odd parity bit,
//! parity bit always one, and parity bit always zero, respectively).
//!
//! The uart peripheral clock is the same as the external clock.
//!
//! \return None.
//
//*****************************************************************************
void
UARTConfigSetExpClk(unsigned long ulBase, unsigned long ulUARTClk,
                    unsigned long ulBaud, unsigned long ulConfig)
{
    unsigned long ulDiv = 0;
    unsigned long ulCompensate = 0;
    unsigned long ulMr = 0;
    unsigned long ulAcr = 0;
    unsigned long ulCLKbr = 0;
    unsigned long ulErrorSum = 0;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    ASSERT(ulBaud != 0);
    ASSERT(ulBaud > 4000000);

    //
    // Stop the UART.
    //
    //UARTDisable(ulBase);

    //
    // Check need compensate or not
    // 
    if ((ulUARTClk % 16 == 0) &&  ulUARTClk % (16 * ulBaud) == 0) {
	    ulCompensate = 0;
	    ulDiv = ulUARTClk / (16 * ulBaud);
    } else {
	    ulCompensate = 1;
    }

    //
    // Compute the fractional baud rate divider.
    // Calculate ulDiv, ulMr and ulAcr
    // Need to adjust ulDiv and ulMr, if ulDiv%16 then ulDiv/=16, ulMr*=16
    //  
    if (ulCompensate) {
        ulCLKbr = BaudRateCLKGet(ulUARTClk, ulBaud ,&ulDiv);
        ulAcr = ACR(ulCLKbr, ulBaud, &ulMr, &ulErrorSum);
        if ((ulDiv % 16) == 0) {
            ulDiv /= 16;
            ulMr *= 16;
        }
    }

    //
    // Set the baud rate.
    // Include Divisor set. UMR set, and Compensate set
    //
    
    //
    // DLAB=1
    //
    UARTDlabSet(ulBase);

    //
    // Set the divisor.
    //
    HWREG(ulBase + UART_O_DLLR) = ulDiv & 0xff;
    //
    // DLAB=1
    //
    UARTDlabSet(ulBase);
    HWREG(ulBase + UART_O_DLHR) = (ulDiv >> 8) & 0xff;

    //
    // Set the UMR.
    //
    HWREG(ulBase + UART_O_MR) = ulMr;

    //
    // Set the Compensate.
    //
    HWREG(ulBase + UART_O_ACR) = ulAcr;

    //
    // Set parity, data length, and number of stop bits.
    //
    HWREG(ulBase + UART_O_LCR) = (HWREG(ulBase + UART_O_LCR) 
	    & (~(UART_CONFIG_WLEN_MASK | UART_CONFIG_STOP_MASK  | UART_CONFIG_PAR_MASK)))
	    | ulConfig;

    //
    // Start the UART.
    //
    //UARTEnable(ulBase);
}

//*****************************************************************************
//
//! Gets the current configuration of a UART.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulUARTClk is the rate of the clock supplied to the UART module.
//! \param pulBaud is a pointer to storage for the baud rate.
//! \param pulConfig is a pointer to storage for the data format.
//!
//! This function determines the baud rate and data format for the UART, given
//! an explicitly provided peripheral clock (hence the ExpClk suffix).  The
//! returned baud rate is the actual baud rate; it may not be the exact baud
//! rate requested or an ``official'' baud rate.  The data format returned in
//! \e pulConfig is enumerated the same as the \e ulConfig parameter of
//! UARTConfigSetExpClk().
//!
//! The peripheral clock is the same as the processor clock.  The frequency of
//! the system clock is the value returned by SysCtlClockGet(), or it can be
//! explicitly hard coded if it is constant and known (to save the
//! code/execution overhead of a call to SysCtlClockGet()).
//!
//! This function replaces the original UARTConfigGet() API and performs the
//! same actions.  A macro is provided in <tt>uart.h</tt> to map the original
//! API to this API.
//!
//! \return None.
//
//*****************************************************************************
void
UARTConfigGetExpClk(unsigned long ulBase, unsigned long ulUARTClk,
                    unsigned long *pulBaud, unsigned long *pulConfig)
{
    unsigned long ulDiv, ulBaudClk, ulBaudClkUnit;

    //   
    // Check the arguments.
    //   
    ASSERT(UARTBaseValid(ulBase));

    //
    // DLAB=1
    //
    UARTDlabSet(ulBase);

    //
    // Compute the baud rate.
    //
    ulDiv = HWREG(ulBase + UART_O_DLLR) | (HWREG(ulBase + UART_O_DLHR) << 8);
    ulBaudClk = ulUARTClk / ulDiv;
    ulBaudClkUnit = HWREG(ulBase + UART_O_MR);
    *pulBaud = ulBaudClk / ulBaudClkUnit;

    //
    // Get the parity, data length, and number of stop bits.
    //
    *pulConfig = (HWREG(ulBase + UART_O_LCR) &
                  (UART_LCR_STPAR | UART_LCR_WLS | UART_LCR_SBLS |
                   UART_LCR_PARM | UART_LCR_PARE));
}

//*****************************************************************************
//
//! Enables transmitting and receiving.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function sets the UARTEN, TXE, and RXE bits and enables the transmit
//! and receive FIFOs.
//!
//! \return None.
//
//*****************************************************************************
void
UARTEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Enable RX, TX, and the UART.
    //
    HWREG(ulBase + UART_O_FCR) = UART_FCR_UME | UART_FCR_FME | UART_FCR_TFRT | UART_FCR_RFRT | UART_FIFO_RX16_64;
}

//*****************************************************************************
//
//! Disables transmitting and receiving.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function clears the UARTEN, TXE, and RXE bits, waits for the end of
//! transmission of the current character, and flushes the transmit FIFO.
//!
//! \return None.
//
//*****************************************************************************
void
UARTDisable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Wait for end of TX.
    //
    while(!(HWREG(ulBase + UART_O_LSR) & UART_LSR_TEMP))
    {
	    (void)ulBase;
    }

    //
    // Reset Tx/Rx FIFO.
    //
    // HWREG(ulBase + UART_O_FCR) &= ~UART_FCR_FME;
    HWREG(ulBase + UART_O_FCR) = UART_FCR_UME | UART_FCR_FME | UART_FCR_TFRT | UART_FCR_RFRT | UART_FIFO_RX16_64;

    //
    // Disable the UART.
    //
    //HWREG(ulBase + UART_O_FCR) &= ~UART_FCR_UME | ~UART_FCR_FME;
    HWREG(ulBase + UART_O_FCR) = 0;
}

//*****************************************************************************
//
//! Enables SIR (IrDA) mode on the specified UART.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function enables the SIREN control bit for IrDA mode on the UART.  If
//! the \e bLowPower flag is set, then SIRLP bit is also set.  This
//! function only has an effect if the UART has not been enabled
//! by a call to UARTEnable().  The call UARTEnableSIR() must be made before
//! a call to UARTConfigSetExpClk() because the UARTConfigSetExpClk() function
//! calls the UARTEnable() function.  Another option is to call UARTDisable()
//! followed by UARTEnableSIR() and then enable the UART by calling
//! UARTEnable().
//!
//! \return None.
//
//*****************************************************************************
void
UARTEnableSIR(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Enable SIR
    //
    HWREG(ulBase + UART_O_ISR) |= (UART_ISR_RCVEIR | UART_ISR_XMITIR);
}

//*****************************************************************************
//
//! Disables SIR (IrDA) mode on the specified UART.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function clears the SIREN (IrDA) and SIRLP (Low Power) bits.  This
//! function only has an effect if the UART has not been enabled by a
//! call to UARTEnable().  The call UARTEnableSIR() must be made before
//! a call to UARTConfigSetExpClk() because the UARTConfigSetExpClk() function
//! calls the UARTEnable() function.  Another option is to call UARTDisable()
//! followed by UARTEnableSIR() and then enable the UART by calling
//! UARTEnable().
//!
//! \note The availability of SIR (IrDA) operation varies with the Stellaris
//! part in use.  Please consult the datasheet for the part you are using to
//! determine whether this support is available.
//!
//! \return None.
//
//*****************************************************************************
void
UARTDisableSIR(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Disable SIR and SIRLP (if appropriate).
    //
    HWREG(ulBase + UART_O_ISR) &= ~(UART_ISR_RCVEIR | UART_ISR_XMITIR);
}

//*****************************************************************************
//
//! Sets the states of the DTR and/or RTS modem control signals.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulControl is a bit-mapped flag indicating which modem control bits
//! should be set.
//!
//! This function configures the states of the DTR or RTS modem handshake
//! outputs from the UART.
//!
//! The \e ulControl parameter is the logical OR of any of the following:
//!
//! - \b UART_MODEM_LOOPBACK - The Modem Control loop back
//! - \b UART_MODEM_RTS - The Modem Control RTS signal
//!
//! \return None.
//
//*****************************************************************************
void
UARTModemControlSet(unsigned long ulBase, unsigned long ulControl)
{
    unsigned long ulTemp;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    ASSERT((ulControl & ~(UART_MODEM_LOOPBACK | UART_MODEM_RTS)) == 0);

    //
    // Set the appropriate modem control output bits.
    //
    ulTemp = HWREG(ulBase + UART_O_MCR);
    ulTemp |= (ulControl & (UART_MODEM_LOOPBACK | UART_MODEM_RTS));
    HWREG(ulBase + UART_O_MCR) = ulTemp;
}

//*****************************************************************************
//
//! Gets the states of the DTR and/or RTS modem control signals.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulControl is a bit-mapped flag indicating which modem control bits
//! should be set.
//!
//! This function gets the states of the DTR or RTS modem handshake
//! outputs from the UART.
//!
//! \return Returns
//! - \b UART_MODEM_LOOPBACK - The Modem loop back Control
//! - \b UART_MODEM_RTS - The Modem Control RTS signal
//
//*****************************************************************************
unsigned long
UARTModemControlGet(unsigned long ulBase)
{
    unsigned long ulTemp;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulTemp = HWREG(ulBase + UART_O_MCR);
    if (ulTemp & UART_MCR_RTS) {
	    return UART_MODEM_RTS;
    }

    if (ulTemp & UART_MCR_LOOP) {
	    return UART_MODEM_LOOPBACK;
    }

    return 0;
}

//*****************************************************************************
//
//! Gets the states of the DTR and/or RTS modem control signals.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulControl is a bit-mapped flag indicating which modem control bits
//! should be set.
//!
//! This function gets the states of the DTR or RTS modem handshake
//! outputs from the UART.
//!
//! \return Returns
//! - \b UART_MODEM_LOOPBACK - The Modem loop back Control
//! - \b UART_MODEM_RTS - The Modem Control RTS signal
//
//*****************************************************************************
tBoolean
UARTModemStatusInterruptEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    // DLAB=0
    UARTDlabClear(ulBase);

    // Enable modem status interrupt
    HWREG(ulBase + UART_O_IER) |= UART_IER_MSIE;


    return((HWREG(ulBase + UART_O_IER) & UART_IER_MSIE) == UART_IER_MSIE);
}
//*****************************************************************************
//
//! Checks TX is empty or not.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function checks TX Transmitter physically is empty or not
//!
//! \return Returns true if TX is empty, false not
//
//*****************************************************************************
tBoolean
UARTTxEmpty(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    
    return(HWREG(ulBase + UART_O_LSR) & UART_LSR_TEMP);
}

//*****************************************************************************
//
//! Starts Rx.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function enables RX Transmitter interrupt
//!
//! \return Returns true if receive line status interrupt is enabled, false not
//
//*****************************************************************************
tBoolean
UARTRxStart(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    // DLAB=0
    UARTDlabClear(ulBase);
    
    // Enable receive timeout interrupt
    HWREG(ulBase + UART_O_IER) |= UART_IER_RTOIE;

    // Enable receive line status interrupt
    HWREG(ulBase + UART_O_IER) &= ~UART_IER_RLSIE;

    // Enable receive data ready interrupt
    HWREG(ulBase + UART_O_IER) |= UART_IER_RDRIE;
    
    // DLAB=0
    UARTDlabClear(ulBase);

    return((HWREG(ulBase + UART_O_IER) & 
			    (UART_IER_RTOIE | UART_IER_RLSIE | UART_IER_RDRIE))
		    == (UART_IER_RTOIE | UART_IER_RLSIE | UART_IER_RDRIE));
}

//*****************************************************************************
//
//! Stops Rx.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function disables RX Transmitter interrupt
//!
//! \return Returns true if receive line status interrupt is disabled, false not
//
//*****************************************************************************
tBoolean
UARTRxStop(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    
    // DLAB=0
    UARTDlabClear(ulBase);

    // Disable receive line status interrupt
    HWREG(ulBase + UART_O_IER) &= ~UART_IER_RLSIE;
    
    // DLAB=0
    UARTDlabClear(ulBase);

    return((HWREG(ulBase + UART_O_IER) & UART_IER_RLSIE) != UART_IER_RLSIE);
}

//*****************************************************************************
//
//! Starts Tx.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function enables TX Transmitter request interrupt, will trigger an Tx
//! interrupt
//!
//! \return Returns true if transmit data request interrupt is enabled, false not
//
//*****************************************************************************
tBoolean
UARTTxStart(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    
    // DLAB=0
    UARTDlabClear(ulBase);

    // Enable the transmit data request interrupt
    HWREG(ulBase + UART_O_IER) |= UART_IER_TDRIE;
    
    // DLAB=0
    UARTDlabClear(ulBase);

    return((HWREG(ulBase + UART_O_IER) & UART_IER_TDRIE) == UART_IER_TDRIE);
}

//*****************************************************************************
//
//! Stops Tx.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function disables TX Transmitter request interrupt
//!
//! \return Returns true if transmit data request interrupt is disabled, false not
//
//*****************************************************************************
tBoolean
UARTTxStop(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));
    
    // DLAB=0
    UARTDlabClear(ulBase);

    // Disable the transmit data request interrupt
    HWREG(ulBase + UART_O_IER) &= ~UART_IER_TDRIE;
    
    // DLAB=0
    UARTDlabClear(ulBase);

    return((HWREG(ulBase + UART_O_IER) & UART_IER_TDRIE) != UART_IER_TDRIE);
}

//*****************************************************************************
//
//! Sends a character to the specified port.
//!
//! \param ulBase is the base address of the UART port.
//! \param ucData is the character to be transmitted.
//!
//! This function writes the character \e ucData to the transmit FIFO for the
//! specified port.  This function does not block, so if there is no space
//! available, then a \b false is returned and the application must retry the
//! function later.
//!
//! This function replaces the original UARTCharNonBlockingPut() API and
//! performs the same actions.  A macro is provided in <tt>uart.h</tt> to map
//! the original API to this API.
//!
//! \return Returns \b true if the character was successfully placed in the
//! transmit FIFO or \b false if there was no space available in the transmit
//! FIFO.
//
//*****************************************************************************
tBoolean
UARTCharPutNonBlocking(unsigned long ulBase, unsigned char ucData)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // See if there is space in the transmit FIFO.
    //
    if((HWREG(ulBase + UART_O_TFCR) & UART_TFCR_TCNT) < UART_FIFO_SIZE)
    {
    	//
    	// DLAB=0
    	//
    	UARTDlabClear(ulBase);

        //
        // Write this character to the transmit FIFO.
        //
        HWREG(ulBase + UART_O_THR) = ucData;

        //
        // Success.
        //
        return(true);
    } else {
        //
        // There is no space in the transmit FIFO, so return a failure.
        //
        return(false);
    }
}

//*****************************************************************************
//
//! Waits to send a character from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//! \param ucData is the character to be transmitted.
//!
//! This function sends the character \e ucData to the transmit FIFO for the
//! specified port.  If there is no space available in the transmit FIFO, this
//! function waits until there is space available before returning.
//!
//! \return None.
//
//*****************************************************************************
void
UARTCharPut(unsigned long ulBase, unsigned char ucData)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Wait until space is available.
    //
    while((HWREG(ulBase + UART_O_TFCR) & UART_TFCR_TCNT) == UART_FIFO_SIZE)
    {
    }

    //
    // DLAB=0
    //
    UARTDlabClear(ulBase);

    //
    // Send the char.
    //
    HWREG(ulBase + UART_O_THR) = ucData;
}

//*****************************************************************************
//
//! Gets a character from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function gets a character from UART_O_RBR. 
//!
//! \return Return a char.
//
//*****************************************************************************
unsigned char
UARTCharGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // DLAB=0
    //
    UARTDlabClear(ulBase);

    //
    // receive a char.
    //
    return(HWREG(ulBase + UART_O_RBR));
}

//*****************************************************************************
//
//! Check if Tx interrupt occured from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulLsr is the value of UART_O_LSR.
//!
//! This function gets value of register UART_O_LSR.UART_LSR_TDRQ
//!
//! \return Return value of register UART_O_LSR.UART_LSR_TDRQ
//
//*****************************************************************************
tBoolean
UARTIsTxInterrupt(unsigned long ulBase, unsigned long ulLsr)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    return(ulLsr & UART_LSR_TDRQ);
}

//*****************************************************************************
//
//! Check Rx interrupt occured from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulLsr is the value of UART_O_LSR.
//!
//! This function gets value of register UART_O_LSR.UART_LSR_DRY
//!
//! \return Return value of register UART_O_LSR.UART_LSR_DRY
//
//*****************************************************************************
tBoolean
UARTIsRxInterrupt(unsigned long ulBase, unsigned long ulLsr)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    return(ulLsr & UART_LSR_DRY);
}

//*****************************************************************************
//
//! Check Rx Break interrupt occured from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulLsr is the value of UART_O_LSR.
//!
//! This function gets value of register UART_O_LSR.UART_LSR_BI
//!
//! \return Return value of register UART_O_LSR.UART_LSR_BI
//
//*****************************************************************************
tBoolean
UARTIsRxBreakInterrupt(unsigned long ulBase, unsigned long ulLsr)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    return(ulLsr & UART_LSR_BI);
}

//*****************************************************************************
//
//! Gets current receiver errors.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function returns the current state of each of the 5 receiver error
//! sources.  The returned errors are equivalent to the four error bits
//! returned via the previous call to UARTCharGet() or UARTCharGetNonBlocking()
//! with the exception that the overrun error is set immediately when the
//! overrun occurs rather than when a character is next read.
//!
//! \return Returns a logical OR combination of the receiver error flags,
//! \b UART_RXERROR_FRAMING, \b UART_RXERROR_PARITY, \b UART_RXERROR_BREAK,
//! \b UART_RXERROR_FIFO and \b UART_RXERROR_OVERRUN.
//
//*****************************************************************************
unsigned long
UARTRxErrorGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Return the current value of the receive status register.
    //
    return(HWREG(ulBase + UART_O_LSR) & UART_RXERROR_MASK);
}

//*****************************************************************************
//
//! Gets value of UART_O_LSR.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function returns the current state of each of the 5 receiver error
//! sources.  The returned errors are equivalent to the four error bits
//! returned via the previous call to UARTCharGet() or UARTCharGetNonBlocking()
//! with the exception that the overrun error is set immediately when the
//! overrun occurs rather than when a character is next read.
//!
//! \return Returns a logical OR combination of the receiver error flags,
//! \b UART_RXERROR_FRAMING, \b UART_RXERROR_PARITY, \b UART_RXERROR_BREAK,
//! \b UART_RXERROR_FIFO and \b UART_RXERROR_OVERRUN.
//
//*****************************************************************************
unsigned long
UARTLsrGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    //
    // Return the current value of the receive status register.
    //
    return(HWREG(ulBase + UART_O_LSR));
}

//*****************************************************************************
// TODO
//! Clear UART_O_LSR from the specified port.
//!
//! \param ulBase is the base address of the UART port.
//!
//! This function clear value of register UART_O_LSR.
//!
//! \return Return UART_O_LSR == 0x00000000.
//
//*****************************************************************************
tBoolean
UARTLsrClear(unsigned long ulBase)
{
    unsigned long ulTemp;
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulTemp = HWREG(ulBase + UART_O_LSR);
    //
    // DLAB=0
    //
    UARTDlabClear(ulBase);
    ulTemp = HWREG(ulBase + UART_O_RBR);
    ulTemp = HWREG(ulBase + UART_O_IIR);
    ulTemp = HWREG(ulBase + UART_O_MSR);

    return(HWREG(ulBase + UART_O_LSR) == 0x00000000);
}

//*****************************************************************************
// 
//! Set UART mode or SIR mode.
//!
//! \param ulBase is the base address of the UART port.
//! \param ulMode UART_MODE_UART or UART_MODE_SIR.
//!
//! This function clear value of register UART_O_LSR.
//!
//! \return Return UART_O_LSR == 0x00000000.
//
//*****************************************************************************
void UARTModeSet(unsigned long ulBase, unsigned long ulMode)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));


    HWREG(ulBase + UART_O_ISR) &= ~UART_MODE_MASK;
    HWREG(ulBase + UART_O_ISR) |= ulMode;
}

__attribute__((unused)) void avoid_compile_error(void)
{
	UARTParityModeGet(0x0);
}
