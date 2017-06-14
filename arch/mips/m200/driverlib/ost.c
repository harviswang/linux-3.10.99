//*****************************************************************************
//
// ost.c - Driver for the OST.
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

#include "../inc/hw_tcu.h"
#include "../inc/hw_ost.h"
#include "../inc/hw_intc.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "debug.h"
#include "intc.h"
#include "ost.h"

//*****************************************************************************
//
// A mapping of OST base address to interupt number.
//
//*****************************************************************************
static const unsigned long g_ppulOSTIntMap[1][2] =
{
    { OST_BASE, IRQ_NO_TCU0 },
};

//*****************************************************************************
//
//! Gets the OST interrupt number.
//!
//! \param ulBase is the base address of the OST.
//!
//! Given a OST base address, this function returns the corresponding
//! interrupt number.
//!
//! \return Returns a OST interrupt number, or -1 if \e ulBase is invalid.
//
//*****************************************************************************
long
OSTIntNumberGet(unsigned long ulBase)
{
    unsigned long ulIdx;

    //
    // Loop through the table that maps OST base addresses to interrupt
    // numbers.
    //
    for(ulIdx = 0; ulIdx < (sizeof(g_ppulOSTIntMap) /
                            sizeof(g_ppulOSTIntMap[0])); ulIdx++)
    {
        //
        // See if this base address matches.
        //
        if(g_ppulOSTIntMap[ulIdx][0] == ulBase)
        {
            //
            // Return the corresponding interrupt number.
            //
            return(g_ppulOSTIntMap[ulIdx][1]);
        }
    }

    //
    // The base address could not be found, so return an error.
    //
    return(-1);
}

//*****************************************************************************
//
//! \internal
//! Checks a OST base address.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function determines if a OST base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
OSTBaseValid(unsigned long ulBase)
{
    return(ulBase == OST_BASE);
}
#endif

//*****************************************************************************
//
//! Set the counter mode of OST.
//!
//! \param ulBase is the base address of the OST.
//! \param ulCounterMode is the counter mode of the OST.
//!        OST_COUNTER_BECLEARED increase to compared value, cleared, increase
//!                              from 0.
//!        OST_COUNTER_GOON increase to compared value, goon increase till
//!                         overflow, increase from 0
//!
//! This function set counter mode.
//!
//! \return None.
//
//*****************************************************************************
void
OSTCounterModeSet(unsigned long ulBase, unsigned long ulCounterMode)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
    ASSERT(ulCounterMode == OST_COUNTER_BECLEARED ||
           ulCounterMode == OST_COUNTER_GOON );

    switch (ulCounterMode) {
    case OST_COUNTER_BECLEARED:
        HWREGH(ulBase + OST_O_CSR) &= ~OST_CSR_CNTMD;
        break;
    case OST_COUNTER_GOON:
        HWREGH(ulBase + OST_O_CSR) |= OST_CSR_CNTMD;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Get the counter mode of OST.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function get the counter mode.
//!
//! \return Returns OST_COUNTER_BECLEARED or OST_COUNTER_GOON.
//
//*****************************************************************************
unsigned long
OSTCounterModeGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    if (HWREGH(ulBase + OST_O_CSR) & OST_CSR_CNTMD) {
        return(OST_COUNTER_GOON);
    } else {
        return(OST_COUNTER_BECLEARED);
    }
}

//*****************************************************************************
//
//! Shutdown the counter of OST.
//!
//! \param ulBase is the base address of the OST.
//! \param ulShutdownMode is the shutdown mode of the OST counter.
//!
//! This function shutdowns OST counter in Graceful/Abrupt mode.
//!
//! \return None.
//
//*****************************************************************************
void
OSTShutdown(unsigned long ulBase, unsigned long ulShutdownMode)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
    ASSERT(ulShutdownMode == OST_SHUTDOWN_GRACEFUL ||
           ulShutdownMode == OST_SHUTDOWN_ABRUPT);

    switch (ulShutdownMode) {
    case OST_SHUTDOWN_GRACEFUL:
        if (OSTCounterModeGet(ulBase) == OST_COUNTER_BECLEARED) {
            HWREGH(ulBase + OST_O_CSR) &= ~OST_CSR_SD;
        } else {
            ASSERT(false);
        }
        break;
    case OST_SHUTDOWN_ABRUPT:
        HWREGH(ulBase + OST_O_CSR) |= OST_CSR_SD;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Set the OST clock input prescale.
//!
//! \param ulBase is the base address of the OST.
//! \param ulClockInputPrescale is the OST clock input prescale.
//!
//! This function set OST clock input prescale.
//!
//! \return None.
//
//*****************************************************************************
void
OSTClockInputPrescaleSet(unsigned long ulBase, unsigned long ulClockInputPrescale)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
    ASSERT(ulClockInputPrescale == OST_CLOCKPRESCALE_1 ||
           ulClockInputPrescale == OST_CLOCKPRESCALE_4 ||
           ulClockInputPrescale == OST_CLOCKPRESCALE_16 ||
           ulClockInputPrescale == OST_CLOCKPRESCALE_64 ||
           ulClockInputPrescale == OST_CLOCKPRESCALE_256 ||
           ulClockInputPrescale == OST_CLOCKPRESCALE_1024);

    switch (ulClockInputPrescale) {
    case OST_CLOCKPRESCALE_1:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_1);
        break;
    case OST_CLOCKPRESCALE_4:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_4);
        break;
    case OST_CLOCKPRESCALE_16:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_16);
        break;
    case OST_CLOCKPRESCALE_64:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_64);
        break;
    case OST_CLOCKPRESCALE_256:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_256);
        break;
    case OST_CLOCKPRESCALE_1024:
        HWREGH(ulBase + OST_O_CSR) &= ((~OST_CSR_PRESCALE) | OST_CSR_PRESCALE_1024);
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Set the clock input of OST.
//!
//! \param ulBase is the base address of the OST.
//! \param ulClockSource is clock input, OST_CLOCKINPUT_EXTAL, 
//! OST_CLOCKINPUT_RTC or OST_CLOCKINPUT_PCLK.
//!
//! This function set the OST clock input.
//!
//! \return None.
//
//*****************************************************************************
void
OSTClockInputSet(unsigned long ulBase, unsigned long ulClockInput)
{
    unsigned long ulMask;
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
    ASSERT(ulClockInput == OST_CLOCKINPUT_EXTAL ||
           ulClockInput == OST_CLOCKINPUT_RTC   ||
           ulClockInput == OST_CLOCKINPUT_PCLK);

    ulMask = OST_CSR_EXTEN | OST_CSR_RTCEN | OST_CSR_PCKEN;

    switch (ulClockInput) {
    case OST_CLOCKINPUT_EXTAL:
        HWREGH(ulBase + OST_O_CSR) &= ~ulMask;
        HWREGH(ulBase + OST_O_CSR) |= OST_CSR_EXTEN;
        break;
    case OST_CLOCKINPUT_RTC:
        HWREGH(ulBase + OST_O_CSR) &= ~ulMask;
        HWREGH(ulBase + OST_O_CSR) |= OST_CSR_RTCEN;
        break;
    case OST_CLOCKINPUT_PCLK:
        HWREGH(ulBase + OST_O_CSR) &= ~ulMask;
        HWREGH(ulBase + OST_O_CSR) |= OST_CSR_PCKEN;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Set the Data Register of OST.
//!
//! \param ulBase is the base address of the OST.
//! \param ulData is the compared data register value.
//!
//! This function set data which TCNT is compared to.
//!
//! \return None.
//
//*****************************************************************************
void
OSTDataSet(unsigned long ulBase, unsigned long ulData)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_DR) = ulData;
}

//*****************************************************************************
//
//! Get the value of OST Data Register.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function get data which TCNT is compared to.
//!
//! \return Returns data register'a value.
//
//*****************************************************************************
unsigned long
OSTDataGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    return(HWREG(ulBase + OST_O_DR));
}

//*****************************************************************************
//
//! Set the Counter Register of OST.
//!
//! \param ulBase is the base address of the OST.
//! \param ulCounterHigh is the high 32-bit of counter.
//! \param ulCounterLow is the low 32-bit of counter.
//!
//! This function set initial value of Counter.
//!
//! \return None.
//
//*****************************************************************************
void
OSTCounterSet(unsigned long ulBase, unsigned long ulCounterHigh, unsigned long ulCounterLow)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_CNTH) = ulCounterHigh;
    HWREG(ulBase + OST_O_CNTL) = ulCounterLow;
}

//*****************************************************************************
//
//! Get the value of OST Counter Register.
//!
//! \param ulBase is the base address of the OST.
//! \param pulCounterHigh store the high 32-bit of counter.
//! \param pulCounterLow store the low 32-bit of counter.
//!
//! This function set data which TCNT is compared to.
//!
//! \return None.
//
//*****************************************************************************
void
OSTCounterGet(unsigned long ulBase, unsigned long *pulCounterHigh, unsigned long *pulCounterLow)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
    ASSERT(pulCounterHigh);
    ASSERT(pulCounterLow);

    // low 32-bit read from OST_O_CNTL
    // must be read before high 32-bit part
    if (pulCounterLow) {
        *pulCounterLow = HWREG(ulBase + OST_O_CNTL);
    }

    // high 32-bit read from OST_O_CNTHBUF
    // note: not from OST_O_CNTH since it maybe changed 
    if (pulCounterHigh) {
        *pulCounterHigh = HWREG(ulBase + OST_O_CNTHBUF);
    }
}

//*****************************************************************************
//
//! Print all register value of OST.
//!
//! \param ulBase is the base address of the OST port.
//! \param print is a function pointer like printf.
//!
//! This function print value of all register in a OST.
//!
//! \return none
//
//*****************************************************************************
void
OSTRegisterDump(unsigned long ulBase, int (*print)(const char *format, ...))
{
    REGH_PRINT(OST_O_ER, ulBase, print);
    REG_PRINT(OST_O_FR, ulBase, print);
    REG_PRINT(OST_O_MR, ulBase, print);
    REG_PRINT(OST_O_DR, ulBase, print);
    REG_PRINT(OST_O_CNTL, ulBase, print);
    REG_PRINT(OST_O_CNTH, ulBase, print);
    REGH_PRINT(OST_O_CSR, ulBase, print);
    REG_PRINT(OST_O_CNTHBUF, ulBase, print);
}

//*****************************************************************************
//
//! Enable OST counter.
//!
//! \param ulBase is the base address of the OST port.
//!
//! This function enable OST counter, begin counting up.
//!
//! \return Returns true if enable operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTCounterEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_ESR) |= OST_ESR_OSTST;

    return((HWREGH(ulBase + OST_O_ER) & OST_ER_OSTEN) == OST_ER_OSTEN);
}

//*****************************************************************************
//
//! Disable OST counter.
//!
//! \param ulBase is the base address of the OST port.
//!
//! This function disable OST counter, stop counting up.
//!
//! \return Returns true if disable operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTCounterDisable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_ECR) |= OST_ECR_OSTCL;

    return((HWREGH(ulBase + OST_O_ER) & OST_ER_OSTEN) != OST_ER_OSTEN);
}

//*****************************************************************************
//
//! Unmask OST interrupt.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function unmask OST comparison match interrupt.
//!
//! \return Returns true if unmask operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTInterruptUnmask(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_MCR) |= OST_MCR_OSTMCL;

    return((HWREG(ulBase + OST_O_MR) & OST_MR_OSTMASK) != OST_MR_OSTMASK);
}

//*****************************************************************************
//
//! Mask OST interrupt.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function mask OST comparison match interrupt.
//!
//! \return Returns true if mask operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTInterruptMask(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_MSR) |= OST_MSR_OSTMST;

    return((HWREG(ulBase + OST_O_MR) & OST_MR_OSTMASK) == OST_MR_OSTMASK);
}

//*****************************************************************************
//
//! OST clock supply.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function supply OST clock, if supplied counter can works.
//!
//! \return Returns true if supply operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTClockSupply(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_SCR) |= OST_SCR_OSTSC;

    return((HWREG(ulBase + OST_O_SR) & OST_SR_OSTS) != OST_SR_OSTS);
}

//*****************************************************************************
//
//! OST clock not supplied(stop).
//!
//! \param ulBase is the base address of the OST.
//!
//! This function not supply OST clock, if not supplied counter can't works.
//!
//! \return Returns true if not supplied operation success, returns false others 
//
//*****************************************************************************
tBoolean
OSTClockNotSupply(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_SSR) |= OST_SSR_OSTSS;

    return((HWREG(ulBase + OST_O_SR) & OST_SR_OSTS) == OST_SR_OSTS);
}

//*****************************************************************************
//
//! OST comparison match flag set.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function set OST comparison match flag, if set an interrupt is occur.
//!
//! \return Returns true if set flag operation success, returns false others
//
//*****************************************************************************
tBoolean
OSTComparisonMatchFlagSet(unsigned long ulBase)
{
    //
    // Checks parameters
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_FSR) |= OST_FSR_OSTFST;
    return((HWREG(ulBase + OST_O_FR) & OST_FR_OSTFLAG) == OST_FR_OSTFLAG);
}

//*****************************************************************************
//
//! OST comparison match flag clear.
//!
//! \param ulBase is the base address of the OST.
//!
//! This function clear OST comparison match flag, if clear an interrupt is finished.
//!
//! \return Returns true if clear flag operation success, returns false others
//
//*****************************************************************************
tBoolean
OSTComparisonMatchFlagClear(unsigned long ulBase)
{
    //
    // Checks parameters
    //
    ASSERT(OSTBaseValid(ulBase));

    HWREG(ulBase + OST_O_FCR) |= OST_FCR_OSTFCL;
    return((HWREG(ulBase + OST_O_FR) & OST_FR_OSTFLAG) != OST_FR_OSTFLAG);
}

