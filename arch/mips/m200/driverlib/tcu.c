//*****************************************************************************
//
// tcu.c - Driver for the TCU.
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
#include "../inc/hw_intc.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "debug.h"
#include "tcu.h"

//*****************************************************************************
//
// A mapping of TCU base address to interupt number.
//
//*****************************************************************************
static const unsigned long g_ppulTCUIntMap[][2] =
{
    { TCU_TIMER0, IRQ_NO_TCU2 },
    { TCU_TIMER1, IRQ_NO_TCU2 },
    { TCU_TIMER2, IRQ_NO_TCU2 },
    { TCU_TIMER3, IRQ_NO_TCU2 },
    { TCU_TIMER4, IRQ_NO_TCU2 },
    { TCU_TIMER5, IRQ_NO_TCU1 },
    { TCU_TIMER6, IRQ_NO_TCU2 },
    { TCU_TIMER7, IRQ_NO_TCU2 },
};

//*****************************************************************************
//
//! \internal
//! Checks a TCU base address.
//!
//! \param ulBase is the base address of the TCU.
//!
//! This function determines if a TCU port base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
TCUBaseValid(unsigned long ulBase)
{
    return(ulBase == TCU_BASE);
}
#endif

//*****************************************************************************
//
//! \internal
//! Checks a TCU Timer ID valid.
//!
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function determines if a TCU timer ID is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
TCUTimerIDValid(unsigned long ulTimerID)
{
    return((ulTimerID == TCU_TIMER0) || (ulTimerID == TCU_TIMER1) || 
           (ulTimerID == TCU_TIMER2) || (ulTimerID == TCU_TIMER3) || 
           (ulTimerID == TCU_TIMER4) || (ulTimerID == TCU_TIMER5) || 
           (ulTimerID == TCU_TIMER6) || (ulTimerID == TCU_TIMER7));
}
#endif

//*****************************************************************************
//
//! Gets the TCU interrupt number.
//!
//! \param ulTimerID is the ID of the TCU timer.
//!
//! Given a TCU Timer ID, this function returns the corresponding
//! interrupt number.
//!
//! \return Returns a TCU interrupt number, or -1 if \e ulTimerID is invalid.
//
//*****************************************************************************
long
TCUIntNumberGet(unsigned long ulTimerID)
{
    unsigned long ulIdx;

    //
    // Loop through the table that maps UART base addresses to interrupt
    // numbers.
    //
    for(ulIdx = 0; ulIdx < (sizeof(g_ppulTCUIntMap) /
                            sizeof(g_ppulTCUIntMap[0])); ulIdx++)
    {
        //
        // See if this base address matches.
        //
        if(g_ppulTCUIntMap[ulIdx][0] == ulTimerID)
        {
            //
            // Return the corresponding interrupt number.
            //
            return(g_ppulTCUIntMap[ulIdx][1]);
        }
    }

    //
    // The base address could not be found, so return an error.
    //
    return(-1);
}

//*****************************************************************************
//
//! Enable a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function 
//!
//! \return Returns a interrupt number, or -1 if \e ulBase is invalid.
//
//*****************************************************************************
void
TCUCounterEnable(unsigned long ulBase, unsigned long ulTimerID)
{
}

//*****************************************************************************
//
//! Shutdown the counter of TCU timer PWM output.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU Timer.
//! \param ulShutdownMode is the shutdown mode of the TCU counter.
//!
//! This function shutdowns OST counter in Graceful/Abrupt mode.
//!
//! \return None.
//
//*****************************************************************************
void
TCUShutdown(unsigned long ulBase, unsigned long ulShutdownMode)
{
    //
    // Check the arguments.
    //
    ASSERT(OSTBaseValid(ulBase));
