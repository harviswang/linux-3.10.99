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
OSTSetCounterMode(unsigned long ulBase, unsigned long ulCounterMode)
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
OSTGetCounterMode(unsigned long ulBase)
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
        if (OSTGetCounterMode(ulBase) == OST_COUNTER_BECLEARED) {
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
OSTSetClockInputPrescale(unsigned long ulBase, unsigned long ulClockInputPrescale)
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
OSTSetClockInput(unsigned long ulBase, unsigned long ulClockInput)
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
OSTSetData(unsigned long ulBase, unsigned long ulData)
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
OSTGetData(unsigned long ulBase)
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
OSTSetCounter(unsigned long ulBase, unsigned long ulCounterHigh, unsigned long ulCounterLow)
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
OSTGetCounter(unsigned long ulBase, unsigned long *pulCounterHigh, unsigned long *pulCounterLow)
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
    REG_PRINT(OST_O_DR, ulBase, print);
    REG_PRINT(OST_O_CNTL, ulBase, print);
    REG_PRINT(OST_O_CNTH, ulBase, print);
    REGH_PRINT(OST_O_CSR, ulBase, print);
    REG_PRINT(OST_O_CNTHBUF, ulBase, print);
}
