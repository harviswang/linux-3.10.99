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
#include "./intc.h"
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
    { TCU_TIMER5, IRQ_NO_TCU1 }, // timer5 use indepedent irq
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
//! \internal
//! Checks a TCU Timer ID is TCU1 valid.
//!
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function determines if a TCU timer ID is TCU1 group valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
TCUTimerTCU1Valid(unsigned long ulTimerID)
{
    return((ulTimerID == TCU_TIMER0) || (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER4) || (ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER6) || (ulTimerID == TCU_TIMER7));
}
#endif

//*****************************************************************************
//
//! \internal
//! Checks a TCU Timer ID is TCU2 valid.
//!
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function determines if a TCU timer ID is TCU1 group valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
TCUTimerTCU2Valid(unsigned long ulTimerID)
{
    return((ulTimerID == TCU_TIMER0) || (ulTimerID == TCU_TIMER3) ||
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
//! This function enable only one TCU timer, when enabled begin counting up
//!
//! \return Returns true if enable operation success, false others.
//
//*****************************************************************************
tBoolean
TCUCounterEnable(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST7;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN7) == TCU_ER_TCEN7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST6;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN6) == TCU_ER_TCEN6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST5;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN5) == TCU_ER_TCEN5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST4;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN4) == TCU_ER_TCEN4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST3;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN3) == TCU_ER_TCEN3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST2;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN2) == TCU_ER_TCEN2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST1;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN1) == TCU_ER_TCEN1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_ESR) |= TCU_ESR_TCST0;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN0) == TCU_ER_TCEN0);
    default:
        ASSERT(false);
        return false;
    }
}

//*****************************************************************************
//
//! Disable a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function disable only one TCU timer, when disabled stop counting up
//!
//! \return Returns true if disable operation success, false others.
//
//*****************************************************************************
tBoolean
TCUCounterDisable(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL7;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN7) != TCU_ER_TCEN7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL6;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN6) != TCU_ER_TCEN6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL5;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN7) != TCU_ER_TCEN7);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL4;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN4) != TCU_ER_TCEN4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL3;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN3) != TCU_ER_TCEN3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL2;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN2) != TCU_ER_TCEN2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL1;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN1) != TCU_ER_TCEN1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_ECR) |= TCU_ECR_TCCL0;
        return((HWREGH(ulBase + TCU_O_ER) & TCU_ER_TCEN0) != TCU_ER_TCEN0);
    default:
        ASSERT(false);
        return false;
    }
}

//*****************************************************************************
//
//! Supply clock of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function supply the clock of TCU timer, as a switch on the clock line.
//! it's just a switch, not any clock input source.
//!
//! \return Returns true if disable operation success, false others.
//
//*****************************************************************************
tBoolean
TCUClockSupply(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC7;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP7) != TCU_SR_STOP7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC6;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP6) != TCU_SR_STOP6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC5;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP5) != TCU_SR_STOP5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC4;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP4) != TCU_SR_STOP4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC3;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP3) != TCU_SR_STOP3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC2;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP2) != TCU_SR_STOP2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC1;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP1) != TCU_SR_STOP1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_SCR) |= TCU_SCR_STPC0;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP0) != TCU_SR_STOP0);
    default:
        ASSERT(false);
        return false;
    }
}

//*****************************************************************************
//
//! \internal
//! Mask a FIFO empty interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function mask an FIFO empty interrupt of a TCU timer.
//!
//! \return Returns true if mask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFIFOEmptyMask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOEMST5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK5) == TCU_MR_FIFOEMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOEMST4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK4) == TCU_MR_FIFOEMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOEMST3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK3) == TCU_MR_FIFOEMASK3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOEMST0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK0) == TCU_MR_FIFOEMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Mask a half comparison interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function mask an half comparison interrupt of a TCU timer.
//!
//! \return Returns true if mask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptHalfMask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST7;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK7) == TCU_MR_HMASK7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST6;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK6) == TCU_MR_HMASK6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK5) == TCU_MR_HMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK4) == TCU_MR_HMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK3) == TCU_MR_HMASK3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST2;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK2) == TCU_MR_HMASK2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST1;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK1) == TCU_MR_HMASK1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_HMST0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK0) == TCU_MR_HMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Mask a FIFO interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function mask an FIFO interrupt of a TCU timer.
//!
//! \return Returns true if mask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFIFOMask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOMST5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK5) == TCU_MR_FIFOMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOMST4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK4) == TCU_MR_FIFOMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOMST3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK3) == TCU_MR_FIFOMASK3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FIFOMST0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK0) == TCU_MR_FIFOMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Mask a full comparison interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function mask an full comparison interrupt of a TCU timer.
//!
//! \return Returns true if mask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFullMask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST7;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK7) == TCU_MR_FMASK7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST6;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK6) == TCU_MR_FMASK6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK5) == TCU_MR_FMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK4) == TCU_MR_FMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK3) == TCU_MR_FMASK3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST2;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK2) == TCU_MR_FMASK2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST1;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK1) == TCU_MR_FMASK1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MSR) |= TCU_MSR_FMST0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK0) == TCU_MR_FMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Mask an interrupt of TCU module.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulMaskType is the type of TCU interrupt.
//!
//! This function mask an kind interrupt of a TCU timer.
//!
//! \return Returns true if mask operation success, false others.
//
//*****************************************************************************
tBoolean
TCUInterruptMask(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMaskType)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulMaskType == TCU_MASKTYPE_FIFOEMPTY) ||
           (ulMaskType == TCU_MASKTYPE_HALF) ||
           (ulMaskType == TCU_MASKTYPE_FIFO) ||
           (ulMaskType == TCU_MASKTYPE_FULL));

    switch (ulMaskType) {
    case TCU_MASKTYPE_FIFOEMPTY:
        return(TCUInterruptFIFOEmptyMask(ulBase, ulTimerID));
    case TCU_MASKTYPE_HALF:
        return(TCUInterruptHalfMask(ulBase, ulTimerID));
    case TCU_MASKTYPE_FIFO:
        return(TCUInterruptFIFOMask(ulBase, ulTimerID));
    case TCU_MASKTYPE_FULL:
        return(TCUInterruptFullMask(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Unmask a FIFO empty interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function unmask an FIFO empty interrupt of a TCU timer.
//!
//! \return Returns true if unmask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFIFOEmptyUnmask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOEMCL5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK5) != TCU_MR_FIFOEMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOEMCL4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK4) != TCU_MR_FIFOEMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOEMCL3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK3) != TCU_MR_FIFOEMASK3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOEMCL0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOEMASK0) != TCU_MR_FIFOEMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Unmask a half comparison interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function unmask an half comparison interrupt of a TCU timer.
//!
//! \return Returns true if unmask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptHalfUnmask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL7;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK7) != TCU_MR_HMASK7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL6;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK6) != TCU_MR_HMASK6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK5) != TCU_MR_HMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK4) != TCU_MR_HMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK3) != TCU_MR_HMASK3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL2;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK2) != TCU_MR_HMASK2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL1;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK1) != TCU_MR_HMASK1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_HMCL0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_HMASK0) != TCU_MR_HMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Unmask a FIFO interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function unmask an FIFO interrupt of a TCU timer.
//!
//! \return Returns true if unmask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFIFOUnmask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));
    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOMCL5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK5) != TCU_MR_FIFOMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOMCL4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK4) != TCU_MR_FIFOMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOMCL3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK3) != TCU_MR_FIFOMASK3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FIFOMCL0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FIFOMASK0) != TCU_MR_FIFOMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Unmask a full comparison interrupt of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function unmask an full comparison interrupt of a TCU timer.
//!
//! \return Returns true if unmask operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUInterruptFullUnmask(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL7;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK7) != TCU_MR_FMASK7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL6;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK6) != TCU_MR_FMASK6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL5;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK5) != TCU_MR_FMASK5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL4;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK4) != TCU_MR_FMASK4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL3;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK3) != TCU_MR_FMASK3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL2;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK2) != TCU_MR_FMASK2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL1;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK1) != TCU_MR_FMASK1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MCR) |= TCU_MCR_FMCL0;
        return((HWREG(ulBase + TCU_O_MR) & TCU_MR_FMASK0) != TCU_MR_FMASK0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Unmask an interrupt of TCU module.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulMaskType is the type of TCU interrupt.
//!
//! This function unmask an kind interrupt of a TCU timer.
//!
//! \return Returns true if unmask operation success, false others.
//
//*****************************************************************************
tBoolean
TCUInterruptUnmask(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMaskType)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulMaskType == TCU_MASKTYPE_FIFOEMPTY) ||
           (ulMaskType == TCU_MASKTYPE_HALF) ||
           (ulMaskType == TCU_MASKTYPE_FIFO) ||
           (ulMaskType == TCU_MASKTYPE_FULL));

    switch (ulMaskType) {
    case TCU_MASKTYPE_FIFOEMPTY:
        return(TCUInterruptFIFOEmptyUnmask(ulBase, ulTimerID));
    case TCU_MASKTYPE_HALF:
        return(TCUInterruptHalfUnmask(ulBase, ulTimerID));
    case TCU_MASKTYPE_FIFO:
        return(TCUInterruptFIFOUnmask(ulBase, ulTimerID));
    case TCU_MASKTYPE_FULL:
        return(TCUInterruptFullUnmask(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set data full regiser of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulDataFull is the value of data full register(just use 16-bit).
//!
//! This function set full register of a TCU timer.
//!
//! \return None.
//
//*****************************************************************************
void
TCUDataFullSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulDataFull)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_DFR7) = ulDataFull;
        break;
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_DFR6) = ulDataFull;
        break;
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_DFR5) = ulDataFull;
        break;
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_DFR4) = ulDataFull;
        break;
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_DFR3) = ulDataFull;
        break;
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_DFR2) = ulDataFull;
        break;
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_DFR1) = ulDataFull;
        break;
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_DFR0) = ulDataFull;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Set data half regiser of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulDataFull is the value of data half register(just use 16-bit).
//!
//! This function set half register of a TCU timer.
//!
//! \return None.
//
//*****************************************************************************
void
TCUDataHalfSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulDataHalf)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_DHR7) = ulDataHalf;
        break;
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_DHR6) = ulDataHalf;
        break;
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_DHR5) = ulDataHalf;
        break;
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_DHR4) = ulDataHalf;
        break;
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_DHR3) = ulDataHalf;
        break;
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_DHR2) = ulDataHalf;
        break;
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_DHR1) = ulDataHalf;
        break;
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_DHR0) = ulDataHalf;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Not supply(Stop) clock of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function stop the clock supply for a TCU timer.
//!
//! \return Returns true if disable operation success, false others.
//
//*****************************************************************************
tBoolean
TCUClockNotSupply(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS7;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP7) == TCU_SR_STOP7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS6;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP6) == TCU_SR_STOP6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS5;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP5) == TCU_SR_STOP5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS4;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP4) == TCU_SR_STOP4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS3;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP3) == TCU_SR_STOP3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS2;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP2) == TCU_SR_STOP2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS1;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP1) == TCU_SR_STOP1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_SSR) |= TCU_SSR_STPS0;
        return((HWREG(ulBase + TCU_O_SR) & TCU_SR_STOP0) == TCU_SR_STOP0);
    default:
        ASSERT(false);
        return false;
    }
}

//*****************************************************************************
//
//! Set the 16-bit counter of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set the counter of a TCU timer.
//!
//! \return None.
//
//*****************************************************************************
void
TCUCounterSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulCounter)
{
    //
    // Check the arguments.
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CNT7) = ulCounter;
        break;
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CNT6) = ulCounter;
        break;
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CNT5) = ulCounter;
        break;
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CNT4) = ulCounter;
        break;
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CNT3) = ulCounter;
        break;
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CNT2) = ulCounter;
        break;
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CNT1) = ulCounter;
        break;
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CNT0) = ulCounter;
        break;
    default:
        ASSERT(false);
        break;
    }
}

//*****************************************************************************
//
//! Shut down(Graceful mode) PWM output of a TCU1 timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function using graceful mode to shut down PWM output of a TCU1 timer.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUPWMShutdownGraceful(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerTCU1Valid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) &= ~TCU_CSR7_SD;
        return((HWREGH(ulBase + TCU_O_CSR7) & TCU_CSR7_SD) != TCU_CSR7_SD);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) &= ~TCU_CSR6_SD;
        return((HWREGH(ulBase + TCU_O_CSR6) & TCU_CSR6_SD) != TCU_CSR6_SD);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) &= ~TCU_CSR5_SD;
        return((HWREGH(ulBase + TCU_O_CSR5) & TCU_CSR5_SD) != TCU_CSR5_SD);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) &= ~TCU_CSR4_SD;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_SD) != TCU_CSR4_SD);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) &= ~TCU_CSR3_SD;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_SD) != TCU_CSR3_SD);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) &= ~TCU_CSR0_SD;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_SD) != TCU_CSR0_SD);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set PWM initial output low level of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set PWM initial output low level.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUPWMInitialOutputLowLevelSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) &= ~TCU_CSR7_INITL;
        return((HWREGH(ulBase + TCU_O_CSR7) & TCU_CSR7_INITL) != TCU_CSR7_INITL);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) &= ~TCU_CSR6_INITL;
        return((HWREGH(ulBase + TCU_O_CSR6) & TCU_CSR6_INITL) != TCU_CSR6_INITL);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) &= ~TCU_CSR5_INITL;
        return((HWREGH(ulBase + TCU_O_CSR5) & TCU_CSR5_INITL) != TCU_CSR5_INITL);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) &= ~TCU_CSR4_INITL;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_INITL) != TCU_CSR4_INITL);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) &= ~TCU_CSR3_INITL;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_INITL) != TCU_CSR3_INITL);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) &= ~TCU_CSR2_INITL;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_INITL) != TCU_CSR2_INITL);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) &= ~TCU_CSR1_INITL;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_INITL) != TCU_CSR1_INITL);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) &= ~TCU_CSR0_INITL;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_INITL) != TCU_CSR0_INITL);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set PWM initial output high level of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set PWM initial output high level.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUPWMInitialOutputHighLevelSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) |= TCU_CSR7_INITL;
        return((HWREGH(ulBase + TCU_O_CSR7) & TCU_CSR7_INITL) == TCU_CSR7_INITL);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) |= TCU_CSR6_INITL;
        return((HWREGH(ulBase + TCU_O_CSR6) & TCU_CSR6_INITL) == TCU_CSR6_INITL);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) |= TCU_CSR5_INITL;
        return((HWREGH(ulBase + TCU_O_CSR5) & TCU_CSR5_INITL) == TCU_CSR5_INITL);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) |= TCU_CSR4_INITL;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_INITL) == TCU_CSR4_INITL);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) |= TCU_CSR3_INITL;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_INITL) == TCU_CSR3_INITL);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) |= TCU_CSR2_INITL;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_INITL) == TCU_CSR2_INITL);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) |= TCU_CSR1_INITL;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_INITL) == TCU_CSR1_INITL);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) |= TCU_CSR0_INITL;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_INITL) == TCU_CSR0_INITL);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Enable PWM output function of a TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function enable PWM output function.
//!
//! \return Returns true if enable PWM operation success, false others.
//
//*****************************************************************************
tBoolean
TCUPWMEnable(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));

    // Not support TCU_TIMER5/TCU_TIMER6/TCU_TIMER7
    ASSERT((ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER2) ||
           (ulTimerID == TCU_TIMER1) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) |= TCU_CSR4_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_PWMEN) == TCU_CSR4_PWMEN);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) |= TCU_CSR3_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_PWMEN) == TCU_CSR3_PWMEN);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) |= TCU_CSR2_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_PWMEN) == TCU_CSR2_PWMEN);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) |= TCU_CSR1_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_PWMEN) == TCU_CSR1_PWMEN);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) |= TCU_CSR0_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_PWMEN) == TCU_CSR0_PWMEN);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Disable PWM output function of a TCU timer(only pure timer function).
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function disable PWM output function.
//!
//! \return Returns true if disable PWM operation success, false others.
//
//*****************************************************************************
tBoolean
TCUPWMDisable(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));

    // Not support TCU_TIMER5/TCU_TIMER6/TCU_TIMER7
    ASSERT((ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER2) ||
           (ulTimerID == TCU_TIMER1) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) &= ~TCU_CSR4_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_PWMEN) != TCU_CSR4_PWMEN);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) &= ~TCU_CSR3_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_PWMEN) != TCU_CSR3_PWMEN);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) &= ~TCU_CSR2_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_PWMEN) != TCU_CSR2_PWMEN);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) &= ~TCU_CSR1_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_PWMEN) != TCU_CSR1_PWMEN);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) &= ~TCU_CSR0_PWMEN;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_PWMEN) != TCU_CSR0_PWMEN);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! PWM initialize output level set.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulPWMInitialOutputLevel is the the TCU PWM initial output level.
//!
//! This function set PWM initial output level of a TCU1 timer.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
tBoolean
TCUPWMInitialOutputLevelSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulPWMInitialOutputLevel)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulPWMInitialOutputLevel == TCU_PWMINITOUTPUT_LOW) ||
           (ulPWMInitialOutputLevel == TCU_PWMINITOUTPUT_HIGH));

    switch (ulPWMInitialOutputLevel) {
    case TCU_PWMINITOUTPUT_LOW:
        return(TCUPWMInitialOutputLowLevelSet(ulBase, ulTimerID));
    case TCU_PWMINITOUTPUT_HIGH:
        return(TCUPWMInitialOutputHighLevelSet(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Shut down(Abrupt mode) PWM output of a TCU1 timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function using abupt mode to shut down PWM output of a TCU1 timer.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUPWMShutdownAbrupt(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerTCU1Valid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) |= TCU_CSR7_SD;
        return((HWREGH(ulBase + TCU_O_CSR7) & TCU_CSR7_SD) == TCU_CSR7_SD);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) |= TCU_CSR6_SD;
        return((HWREGH(ulBase + TCU_O_CSR6) & TCU_CSR6_SD) == TCU_CSR6_SD);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) |= TCU_CSR5_SD;
        return((HWREGH(ulBase + TCU_O_CSR5) & TCU_CSR5_SD) == TCU_CSR5_SD);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) |= TCU_CSR4_SD;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_SD) == TCU_CSR4_SD);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) |= TCU_CSR3_SD;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_SD) == TCU_CSR3_SD);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) |= TCU_CSR2_SD;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_SD) == TCU_CSR2_SD);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) |= TCU_CSR1_SD;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_SD) == TCU_CSR1_SD);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) |= TCU_CSR0_SD;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_SD) == TCU_CSR0_SD);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Shut down PWM output of a TCU1 timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulShutdown is the shut down mode, graceful or abrupt.
//!
//! This function shut down PWM output of a TCU1 timer.
//!
//! \return Returns true if shutdown operation success, false others.
//
//*****************************************************************************
tBoolean
TCUPWMShutdown(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulShutdown)
{
    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerTCU1Valid(ulTimerID));
    ASSERT((ulShutdown == TCU_SHUTDOWN_GRACEFUL) ||
           (ulShutdown == TCU_SHUTDOWN_ABRUPT));

    switch (ulShutdown) {
    case TCU_SHUTDOWN_GRACEFUL:
        return(TCUPWMShutdownGraceful(ulBase, ulTimerID));
    case TCU_SHUTDOWN_ABRUPT:
        return(TCUPWMShutdownAbrupt(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the TCU Timer clock input prescale.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulClockInputPrescale is the OST clock input prescale.
//!
//! This function set TCU Timer clock input prescale.
//!
//! \return None.
//
//*****************************************************************************
tBoolean
TCUClockInputPrescaleSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulClockInputPrescale)
{
    unsigned long prescale_field = 0;

    //
    // Check parameters
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulClockInputPrescale == TCU_CLOCKPRESCALE_1) ||
           (ulClockInputPrescale == TCU_CLOCKPRESCALE_4) ||
           (ulClockInputPrescale == TCU_CLOCKPRESCALE_16) ||
           (ulClockInputPrescale == TCU_CLOCKPRESCALE_64) ||
           (ulClockInputPrescale == TCU_CLOCKPRESCALE_256) ||
           (ulClockInputPrescale == TCU_CLOCKPRESCALE_1024));

    switch (ulClockInputPrescale) {
    case TCU_CLOCKPRESCALE_1:
        prescale_field = TCU_CSR_PRESCALE_1;
        break;
    case TCU_CLOCKPRESCALE_4:
        prescale_field = TCU_CSR_PRESCALE_4;
        break;
    case TCU_CLOCKPRESCALE_16:
        prescale_field = TCU_CSR_PRESCALE_16;
        break;
    case TCU_CLOCKPRESCALE_64:
        prescale_field = TCU_CSR_PRESCALE_64;
        break;
    case TCU_CLOCKPRESCALE_256:
        prescale_field = TCU_CSR_PRESCALE_256;
        break;
    case TCU_CLOCKPRESCALE_1024:
        prescale_field = TCU_CSR_PRESCALE_1024;
        break;
    default:
        ASSERT(false);
        return(false);
    }

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) &= ~TCU_CSR7_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR7) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR7) & TCU_CSR7_PRESCALE) == prescale_field);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) &= ~TCU_CSR6_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR6) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR6) & TCU_CSR6_PRESCALE) == prescale_field);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) &= ~TCU_CSR5_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR5) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR5) & TCU_CSR5_PRESCALE) == prescale_field);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) &= ~TCU_CSR4_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR4) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR4) & TCU_CSR4_PRESCALE) == prescale_field);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) &= ~TCU_CSR3_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR3) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR3) & TCU_CSR3_PRESCALE) == prescale_field);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) &= ~TCU_CSR2_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR2) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR2) & TCU_CSR2_PRESCALE) == prescale_field);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) &= ~TCU_CSR1_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR1) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR1) & TCU_CSR1_PRESCALE) == prescale_field);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) &= ~TCU_CSR0_PRESCALE;
        HWREGH(ulBase + TCU_O_CSR0) |= prescale_field;
        return((HWREGH(ulBase + TCU_O_CSR0) & TCU_CSR0_PRESCALE) == prescale_field);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the TCU Timer clock input(clock source).
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulClockInputPrescale is the OST clock input prescale.
//!
//! This function set TCU Timer clock input.
//!
//! \return None.
//
//*****************************************************************************
tBoolean
TCUClockInputSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulClockInput)
{
    unsigned long ulMask=0;
    unsigned long ulPrescale = 0;

    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT(ulClockInput == TCU_CLOCKINPUT_EXTAL ||
           ulClockInput == TCU_CLOCKINPUT_RTC   ||
           ulClockInput == TCU_CLOCKINPUT_PCLK);

    ulMask = TCU_CSR0_EXTEN | TCU_CSR0_RTCEN | TCU_CSR0_PCKEN;

    switch (ulClockInput) {
    case TCU_CLOCKINPUT_EXTAL:
        ulPrescale = TCU_CSR0_EXTEN;
        break;
    case TCU_CLOCKINPUT_RTC:
        ulPrescale = TCU_CSR0_RTCEN;
        break;
    case TCU_CLOCKINPUT_PCLK:
        ulPrescale = TCU_CSR0_PCKEN;
        break;
    default:
        ASSERT(false);
        return(false);
    }

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREGH(ulBase + TCU_O_CSR7) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR7) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR7) & ulMask) == ulPrescale);
    case TCU_TIMER6:
        HWREGH(ulBase + TCU_O_CSR6) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR6) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR6) & ulMask) == ulPrescale);
    case TCU_TIMER5:
        HWREGH(ulBase + TCU_O_CSR5) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR5) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR5) & ulMask) == ulPrescale);
    case TCU_TIMER4:
        HWREGH(ulBase + TCU_O_CSR4) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR4) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR4) & ulMask) == ulPrescale);
    case TCU_TIMER3:
        HWREGH(ulBase + TCU_O_CSR3) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR3) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR3) & ulMask) == ulPrescale);
    case TCU_TIMER2:
        HWREGH(ulBase + TCU_O_CSR2) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR2) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR2) & ulMask) == ulPrescale);
    case TCU_TIMER1:
        HWREGH(ulBase + TCU_O_CSR1) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR1) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR1) & ulMask) == ulPrescale);
    case TCU_TIMER0:
        HWREGH(ulBase + TCU_O_CSR0) &= ~ulMask;
        HWREGH(ulBase + TCU_O_CSR0) |= ulPrescale;
        return((HWREGH(ulBase + TCU_O_CSR0) & ulMask) == ulPrescale);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the TCU fifo empty comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set TCU fifo empty comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFIFOEmptyMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOESET5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG5) == TCU_FR_FIFOEFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOESET4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG4) == TCU_FR_FIFOEFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOESET3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG3) == TCU_FR_FIFOEFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOESET0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG0) == TCU_FR_FIFOEFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the TCU half comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set TCU half comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonHalfMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST7;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG7) == TCU_FR_HFLAG7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST6;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG6) == TCU_FR_HFLAG6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG5) == TCU_FR_HFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG4) == TCU_FR_HFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG3) == TCU_FR_HFLAG3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST2;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG2) == TCU_FR_HFLAG2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST1;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG1) == TCU_FR_HFLAG1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_HFST0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG0) == TCU_FR_HFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the TCU fifo comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set TCU fifo comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFIFOMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOST5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG5) == TCU_FR_FIFOFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOST4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG4) == TCU_FR_FIFOFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOST3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG3) == TCU_FR_FIFOFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FIFOST0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG0) == TCU_FR_FIFOFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the TCU full comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function set TCU full comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFullMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerTCU1Valid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST7;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG7) == TCU_FR_FFLAG7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST6;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG6) == TCU_FR_FFLAG6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG5) == TCU_FR_FFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG4) == TCU_FR_FFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG3) == TCU_FR_FFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FSR) |= TCU_FSR_FFST0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG0) == TCU_FR_FFLAG0);
    case TCU_TIMER2:
    case TCU_TIMER1:
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the TCU comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//! \param ulFlagType is the type of the TCU timer flag, full/half/fifoempty/fifo.
//!
//! This function set TCU comparison match flag.
//!
//! \return .
//
//*****************************************************************************
tBoolean
TCUComparisonMatchFlagSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFlagType)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulFlagType == TCU_FLAGTYPE_FIFOEMPTY) ||
           (ulFlagType == TCU_FLAGTYPE_HALF) ||
           (ulFlagType == TCU_FLAGTYPE_FIFOFULL) ||
           (ulFlagType == TCU_FLAGTYPE_FULL));


    switch (ulFlagType) {
    case TCU_FLAGTYPE_FIFOEMPTY:
        return(TCUComparisonFIFOEmptyMatchFlagSet(ulBase, ulTimerID));
    case TCU_FLAGTYPE_HALF:
        return(TCUComparisonHalfMatchFlagSet(ulBase, ulTimerID));
    case TCU_FLAGTYPE_FIFOFULL:
        return(TCUComparisonFIFOMatchFlagSet(ulBase, ulTimerID));
    case TCU_FLAGTYPE_FULL:
        return(TCUComparisonFullMatchFlagSet(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Clear the TCU fifo empty comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function clear TCU fifo empty comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFIFOEmptyMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOECL5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG5) != TCU_FR_FIFOEFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOECL4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG4) != TCU_FR_FIFOEFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOECL3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG3) != TCU_FR_FIFOEFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOECL0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOEFLAG0) != TCU_FR_FIFOEFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Clear the TCU half comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function clear TCU half comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonHalfMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL7;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG7) != TCU_FR_HFLAG7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL6;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG6) != TCU_FR_HFLAG6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG5) != TCU_FR_HFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG4) != TCU_FR_HFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG3) != TCU_FR_HFLAG3);
    case TCU_TIMER2:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL2;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG2) != TCU_FR_HFLAG2);
    case TCU_TIMER1:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL1;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG1) != TCU_FR_HFLAG1);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_HFCL0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_HFLAG0) != TCU_FR_HFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Clear the TCU fifo comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function clear TCU fifo comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFIFOMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOCL5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG5) != TCU_FR_FIFOFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOCL4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG4) != TCU_FR_FIFOFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOCL3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG3) != TCU_FR_FIFOFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FIFOCL0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FIFOFLAG0) != TCU_FR_FIFOFLAG0);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Clear the TCU full comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function clear TCU full comparison match flag.
//!
//! \return Returns true if success, flase others.
//
//*****************************************************************************
static tBoolean
TCUComparisonFullMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerTCU1Valid(ulTimerID));

    switch (ulTimerID) {
    case TCU_TIMER7:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL7;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG7) != TCU_FR_FFLAG7);
    case TCU_TIMER6:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL6;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG6) != TCU_FR_FFLAG6);
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL5;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG5) != TCU_FR_FFLAG5);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL4;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG4) != TCU_FR_FFLAG4);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL3;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG3) != TCU_FR_FFLAG3);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FCR) |= TCU_FCR_FFCL0;
        return((HWREG(ulBase + TCU_O_FR) & TCU_FR_FFLAG0) != TCU_FR_FFLAG0);
    case TCU_TIMER2:
    case TCU_TIMER1:
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Clear the TCU comparison match flag.
//!
//! \param ulBase is the base address of the OST.
//! \param ulTimerID is the ID of the TCU timer.
//!
//! This function clear TCU comparison match flag.
//!
//! \return .
//
//*****************************************************************************
tBoolean
TCUComparisonMatchFlagClear(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFlagType)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT(TCUTimerIDValid(ulTimerID));
    ASSERT((ulFlagType == TCU_FLAGTYPE_FIFOEMPTY) ||
           (ulFlagType == TCU_FLAGTYPE_HALF) ||
           (ulFlagType == TCU_FLAGTYPE_FIFOFULL) ||
           (ulFlagType == TCU_FLAGTYPE_FULL));


    switch (ulFlagType) {
    case TCU_FLAGTYPE_FIFOEMPTY:
        return(TCUComparisonFIFOEmptyMatchFlagClear(ulBase, ulTimerID));
    case TCU_FLAGTYPE_HALF:
        return(TCUComparisonHalfMatchFlagClear(ulBase, ulTimerID));
    case TCU_FLAGTYPE_FIFOFULL:
        return(TCUComparisonFIFOMatchFlagClear(ulBase, ulTimerID));
    case TCU_FLAGTYPE_FULL:
        return(TCUComparisonFullMatchFlagClear(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Print all register value of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param print is a function pointer like printf.
//!
//! This function print value of all register in a TCU.
//!
//! \return none
//
//*****************************************************************************
void
TCURegisterDump(unsigned long ulBase, int (*print)(const char *format, ...))
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));

    REGH_PRINT(TCU_O_ER, ulBase, print);
    REG_PRINT(TCU_O_SR, ulBase, print);
    REG_PRINT(TCU_O_FR, ulBase, print);
    REG_PRINT(TCU_O_MR, ulBase, print);
    REGH_PRINT(TCU_O_DFR0, ulBase, print);
    REGH_PRINT(TCU_O_DHR0, ulBase, print);
    REGH_PRINT(TCU_O_CNT0, ulBase, print);
    REGH_PRINT(TCU_O_CSR0, ulBase, print);
    REGH_PRINT(TCU_O_DFR1, ulBase, print);
    REGH_PRINT(TCU_O_DHR1, ulBase, print);
    REGH_PRINT(TCU_O_CNT1, ulBase, print);
    REGH_PRINT(TCU_O_CSR1, ulBase, print);
    REGH_PRINT(TCU_O_DFR2, ulBase, print);
    REGH_PRINT(TCU_O_DHR2, ulBase, print);
    REGH_PRINT(TCU_O_CNT2, ulBase, print);
    REGH_PRINT(TCU_O_CSR2, ulBase, print);
    REGH_PRINT(TCU_O_DFR3, ulBase, print);
    REGH_PRINT(TCU_O_DHR3, ulBase, print);
    REGH_PRINT(TCU_O_CNT3, ulBase, print);
    REGH_PRINT(TCU_O_CSR3, ulBase, print);
    REGH_PRINT(TCU_O_DFR4, ulBase, print);
    REGH_PRINT(TCU_O_DHR4, ulBase, print);
    REGH_PRINT(TCU_O_CNT4, ulBase, print);
    REGH_PRINT(TCU_O_CSR4, ulBase, print);
    REGH_PRINT(TCU_O_DFR5, ulBase, print);
    REGH_PRINT(TCU_O_DHR5, ulBase, print);
    REGH_PRINT(TCU_O_CNT5, ulBase, print);
    REGH_PRINT(TCU_O_CSR5, ulBase, print);
    REGH_PRINT(TCU_O_DFR6, ulBase, print);
    REGH_PRINT(TCU_O_DHR6, ulBase, print);
    REGH_PRINT(TCU_O_CNT6, ulBase, print);
    REGH_PRINT(TCU_O_CSR6, ulBase, print);
    REGH_PRINT(TCU_O_DFR7, ulBase, print);
    REGH_PRINT(TCU_O_DHR7, ulBase, print);
    REGH_PRINT(TCU_O_CNT7, ulBase, print);
    REGH_PRINT(TCU_O_CSR7, ulBase, print);
    REG_PRINT(TCU_O_STR, ulBase, print);
    REGH_PRINT(TCU_O_MODE0, ulBase, print);
    REGH_PRINT(TCU_O_FWD0, ulBase, print);
    REGH_PRINT(TCU_O_FWD0, ulBase, print);
    REGH_PRINT(TCU_O_FIFOSR0, ulBase, print);
    REGH_PRINT(TCU_O_MODE3, ulBase, print);
    REGH_PRINT(TCU_O_FWD3, ulBase, print);
    REGH_PRINT(TCU_O_FWD3, ulBase, print);
    REGH_PRINT(TCU_O_FIFOSR3, ulBase, print);
    REGH_PRINT(TCU_O_MODE4, ulBase, print);
    REGH_PRINT(TCU_O_FWD4, ulBase, print);
    REGH_PRINT(TCU_O_FWD4, ulBase, print);
    REGH_PRINT(TCU_O_FIFOSR4, ulBase, print);
    REGH_PRINT(TCU_O_MODE5, ulBase, print);
    REGH_PRINT(TCU_O_FWD5, ulBase, print);
    REGH_PRINT(TCU_O_FWD5, ulBase, print);
    REGH_PRINT(TCU_O_FIFOSR5, ulBase, print);
}

//*****************************************************************************
//
//! \internal
//! Set the nonfifo mode of TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//!
//! This function set non fifo mode of a TCU timer.
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
static tBoolean
TCUModeNonFIFOSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(TCU_BASE + TCU_O_MODE5) &= ~TCU_MODE5_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE5) & TCU_MODE5_FIFOMOD) != TCU_MODE5_FIFOMOD);
    case TCU_TIMER4:
        HWREG(TCU_BASE + TCU_O_MODE4) &= ~TCU_MODE4_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE4) & TCU_MODE4_FIFOMOD) != TCU_MODE4_FIFOMOD);
    case TCU_TIMER3:
        HWREG(TCU_BASE + TCU_O_MODE3) &= ~TCU_MODE3_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE3) & TCU_MODE3_FIFOMOD) != TCU_MODE3_FIFOMOD);
    case TCU_TIMER0:
        HWREG(TCU_BASE + TCU_O_MODE0) &= ~TCU_MODE0_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE0) & TCU_MODE0_FIFOMOD) != TCU_MODE0_FIFOMOD);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the FIFO mode of TCU timer.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//!
//! This function set FIFO mode of a TCU timer.
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
static tBoolean
TCUModeFIFOSet(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(TCU_BASE + TCU_O_MODE5) |= TCU_MODE5_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE5) & TCU_MODE5_FIFOMOD) == TCU_MODE5_FIFOMOD);
    case TCU_TIMER4:
        HWREG(TCU_BASE + TCU_O_MODE4) |= TCU_MODE4_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE4) & TCU_MODE4_FIFOMOD) == TCU_MODE4_FIFOMOD);
    case TCU_TIMER3:
        HWREG(TCU_BASE + TCU_O_MODE3) |= TCU_MODE3_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE3) & TCU_MODE3_FIFOMOD) == TCU_MODE3_FIFOMOD);
    case TCU_TIMER0:
        HWREG(TCU_BASE + TCU_O_MODE0) |= TCU_MODE0_FIFOMOD;
        return((HWREG(TCU_BASE + TCU_O_MODE0) & TCU_MODE0_FIFOMOD) == TCU_MODE0_FIFOMOD);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the mode of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulMode is the mode of TCU timer.
//!
//! This function set a mode of a TCU timer, mode can be FIFO or NONFIFO.
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
tBoolean
TCUModeSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulMode)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));
    ASSERT((ulMode == TCU_MODE_NONFIFO) ||
           (ulMode == TCU_MODE_FIFO));

    switch (ulMode) {
    case TCU_MODE_NONFIFO:
        return(TCUModeNonFIFOSet(ulBase, ulTimerID));
    case TCU_MODE_FIFO:
        return(TCUModeFIFOSet(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the FIFO1 mode of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//!
//! This function set a FIFO(FIFO1) mode of a TCU timer
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
static tBoolean
TCUFIFOModeFIFO1Set(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) &= ~TCU_MODE5_CYCST;
        return((HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_CYCST) != TCU_MODE5_CYCST);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MODE4) &= ~TCU_MODE4_CYCST;
        return((HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_CYCST) != TCU_MODE4_CYCST);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MODE3) &= ~TCU_MODE3_CYCST;
        return((HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_CYCST) != TCU_MODE3_CYCST);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_CYCST;
        return((HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_CYCST) != TCU_MODE0_CYCST);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set the FIFO2 mode of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//!
//! This function set a FIFO(FIFO2) mode of a TCU timer.
//!
//! \return Return true if set operation success, false others.
//
//*****************************************************************************
static tBoolean
TCUFIFOModeFIFO2Set(unsigned long ulBase, unsigned long ulTimerID)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) |= TCU_MODE5_CYCST;
        return((HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_CYCST) == TCU_MODE5_CYCST);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MODE4) |= TCU_MODE4_CYCST;
        return((HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_CYCST) == TCU_MODE4_CYCST);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MODE3) |= TCU_MODE3_CYCST;
        return((HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_CYCST) == TCU_MODE3_CYCST);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MODE0) |= TCU_MODE0_CYCST;
        return((HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_CYCST) == TCU_MODE0_CYCST);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the FIFO(1/2)mode of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulFIFOMode is the FIFO mode of TCU timer.
//!
//! This function set a FIFO mode of a TCU timer, mode can be FIFO1 or FIFO2.
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFOModeSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFIFOMode)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));
    ASSERT((ulFIFOMode == TCU_FIFOMODE_FIFO1) ||
           (ulFIFOMode == TCU_FIFOMODE_FIFO2));

    switch (ulFIFOMode) {
    case TCU_FIFOMODE_FIFO1:
        return(TCUFIFOModeFIFO1Set(ulBase, ulTimerID));
    case TCU_FIFOMODE_FIFO2:
        return(TCUFIFOModeFIFO2Set(ulBase, ulTimerID));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set the FIFO2 number of TCU.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulNumber is number(size) of FIFO2 TCU timer mode.
//!
//! This function set a FIFO2 FIFO number of a TCU timer.
//!
//! \return Return true if set operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFO2NumberSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulNumber)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) |= TCU_MODE5_FIFONUMST;
        HWREG(ulBase + TCU_O_MODE5) |= ulNumber << 4;
        return(false);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Clear TCU Timer FIFO.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//!
//! This function clear the FIFO of a TCU timer.
//!
//! \return Return true if clear operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFOClear(unsigned long ulBase, unsigned long ulTimerID)
{
    tBoolean x, y;

    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) |=  TCU_MODE5_FIFOCLR;
        x = ((HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_FIFOCLR) == TCU_MODE5_FIFOCLR);
        HWREG(ulBase + TCU_O_MODE5) &= ~TCU_MODE5_FIFOCLR;
        y = ((HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_FIFOCLR) != TCU_MODE5_FIFOCLR);
        return(x & y);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MODE4) |=  TCU_MODE4_FIFOCLR;
        x = ((HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_FIFOCLR) == TCU_MODE4_FIFOCLR);
        HWREG(ulBase + TCU_O_MODE4) &= ~TCU_MODE4_FIFOCLR;
        y = ((HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_FIFOCLR) != TCU_MODE4_FIFOCLR);
        return(x & y);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MODE3) |=  TCU_MODE3_FIFOCLR;
        x = ((HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_FIFOCLR) == TCU_MODE3_FIFOCLR);
        HWREG(ulBase + TCU_O_MODE3) &= ~TCU_MODE3_FIFOCLR;
        y = ((HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_FIFOCLR) != TCU_MODE3_FIFOCLR);
        return(x & y);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MODE0) |=  TCU_MODE0_FIFOCLR;
        x = ((HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_FIFOCLR) == TCU_MODE0_FIFOCLR);
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_FIFOCLR;
        y = ((HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_FIFOCLR) != TCU_MODE0_FIFOCLR);
        return(x & y);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Write a 32-bit data to TCU Timer FIFO.
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulData is 32-bit data(both high and low 16-bit can't be zero).
//!
//! This function write data to the FIFO of a TCU timer.
//!
//! \return Return true if fifo write operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFOWrite(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulData)
{
    unsigned short usHigh, usLow;

    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));
    ASSERT(((ulData & 0xFFFF) != 0x0) || ((ulData >> 16) != 0x0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_FWD5) = ulData;
        usLow  = HWREGH(ulBase + TCU_O_FWD5);
        usHigh = HWREGH(ulBase + TCU_O_FWD5);
        return(((usHigh << 16) | usLow) == ulData);
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_FWD4) = ulData;
        usLow  = HWREGH(ulBase + TCU_O_FWD4);
        usHigh = HWREGH(ulBase + TCU_O_FWD4);
        return(((usHigh << 16) | usLow) == ulData);
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_FWD3) = ulData;
        usLow  = HWREGH(ulBase + TCU_O_FWD3);
        usHigh = HWREGH(ulBase + TCU_O_FWD3);
        return(((usHigh << 16) | usLow) == ulData);
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_FWD0) = ulData;
        usLow  = HWREGH(ulBase + TCU_O_FWD0);
        usHigh = HWREGH(ulBase + TCU_O_FWD0);
        return(((usHigh << 16) | usLow) == ulData);
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set FIFO number(only works in FIFO mode 2).
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulFIFONumber is 32-bit number, less or equal 16.
//!
//! This function write data to the FIFO of a TCU timer.
//!
//! \return Return true if fifo number set operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFONumberSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFIFONumber)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));
    ASSERT(ulFIFONumber <= TCU_FIFO_LENGTH);

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) |=  TCU_MODE5_FIFONUMST;
        HWREG(ulBase + TCU_O_MODE5) &= ~TCU_MODE5_FIFONUM;
        HWREG(ulBase + TCU_O_MODE5) |= ((ulFIFONumber << 4) & TCU_MODE5_FIFONUM);
        return(( HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_FIFONUM) == (ulFIFONumber << 4));
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MODE4) |=  TCU_MODE4_FIFONUMST;
        HWREG(ulBase + TCU_O_MODE4) &= ~TCU_MODE4_FIFONUM;
        HWREG(ulBase + TCU_O_MODE4) |= ((ulFIFONumber << 4) & TCU_MODE4_FIFONUM);
        return(( HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_FIFONUM) == (ulFIFONumber << 4));
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MODE3) |=  TCU_MODE3_FIFONUMST;
        HWREG(ulBase + TCU_O_MODE3) &= ~TCU_MODE3_FIFONUM;
        HWREG(ulBase + TCU_O_MODE3) |= ((ulFIFONumber << 4) & TCU_MODE3_FIFONUM);
        return(( HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_FIFONUM) == (ulFIFONumber << 4));
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MODE0) |=  TCU_MODE0_FIFONUMST;
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_FIFONUM;
        HWREG(ulBase + TCU_O_MODE0) |= ((ulFIFONumber << 4) & TCU_MODE0_FIFONUM);
        return(( HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_FIFONUM) == (ulFIFONumber << 4));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Set FIFO cycle number(only works in FIFO mode 2).
//!
//! \param ulBase is the base address of the TCU.
//! \param ulTimerID is the ID of a TCU timer.
//! \param ulFIFOCycleNumber is 32-bit number.
//!
//! This function write the FIFO cycle of a TCU timer.
//! cycle number means the FIFO data will be used times.
//!
//! \return Return true if fifo cycle number set operation success, false others
//
//*****************************************************************************
tBoolean
TCUFIFOCycleNumberSet(unsigned long ulBase, unsigned long ulTimerID, unsigned long ulFIFOCycleNumber)
{
    //
    // Checks the arguments
    //
    ASSERT(TCUBaseValid(ulBase));
    ASSERT((ulTimerID == TCU_TIMER5) ||
           (ulTimerID == TCU_TIMER4) ||
           (ulTimerID == TCU_TIMER3) ||
           (ulTimerID == TCU_TIMER0));

    switch (ulTimerID) {
    case TCU_TIMER5:
        HWREG(ulBase + TCU_O_MODE5) |=  TCU_MODE5_CYCST;
        HWREG(ulBase + TCU_O_MODE5) &= ~TCU_MODE5_CYCNUM;
        HWREG(ulBase + TCU_O_MODE5) |= ((ulFIFOCycleNumber << 10) & TCU_MODE5_CYCNUM);
        return((HWREG(ulBase + TCU_O_MODE5) & TCU_MODE5_CYCNUM) == (ulFIFOCycleNumber << 10));
    case TCU_TIMER4:
        HWREG(ulBase + TCU_O_MODE4) |=  TCU_MODE4_CYCST;
        HWREG(ulBase + TCU_O_MODE4) &= ~TCU_MODE4_CYCNUM;
        HWREG(ulBase + TCU_O_MODE4) |= ((ulFIFOCycleNumber << 10) & TCU_MODE4_CYCNUM);
        return((HWREG(ulBase + TCU_O_MODE4) & TCU_MODE4_CYCNUM) == (ulFIFOCycleNumber << 10));
    case TCU_TIMER3:
        HWREG(ulBase + TCU_O_MODE3) |=  TCU_MODE3_CYCST;
        HWREG(ulBase + TCU_O_MODE3) &= ~TCU_MODE3_CYCNUM;
        HWREG(ulBase + TCU_O_MODE3) |= ((ulFIFOCycleNumber << 10) & TCU_MODE3_CYCNUM);
        return((HWREG(ulBase + TCU_O_MODE3) & TCU_MODE3_CYCNUM) == (ulFIFOCycleNumber << 10));
    case TCU_TIMER0:
        HWREG(ulBase + TCU_O_MODE0) |=  TCU_MODE0_CYCST;
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_CYCNUM;
        HWREG(ulBase + TCU_O_MODE0) |= ((ulFIFOCycleNumber << 10) & TCU_MODE0_CYCNUM);
        return((HWREG(ulBase + TCU_O_MODE0) & TCU_MODE0_CYCNUM) == (ulFIFOCycleNumber << 10));
    default:
        ASSERT(false);
        return(false);
    }
}
