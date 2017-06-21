//*****************************************************************************
//
// pwm.c - Driver for the PWM(the submodule of TCU).
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

#include "tcu.h"
#include "pwm.h"
#include "debug.h"
#include "gpio.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_tcu.h"

//*****************************************************************************
//
//! \internal
//! Checks a PWM base address.
//!
//! \param ulBase is the base address of the PWM.
//!
//! This function determines if a PWM base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
PWMBaseValid(unsigned long ulBase)
{
    return(TCUBaseValid(ulBase));
}
#endif

//*****************************************************************************
//
//! \internal
//! Checks a PWM index.
//!
//! \param ulIndex is the index of the PWM.
//!
//! This function determines if a PWM index is valid.
//!
//! \return Returns \b true if the index number is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
PWMIndexValid(unsigned long ulIndex)
{
    return((ulIndex == PWM_INDEX_0) ||
           (ulIndex == PWM_INDEX_1) ||
           (ulIndex == PWM_INDEX_2) ||
           (ulIndex == PWM_INDEX_3) ||
           (ulIndex == PWM_INDEX_4) ||
           (ulIndex == PWM_INDEX_5) ||
           (ulIndex == PWM_INDEX_6) ||
           (ulIndex == PWM_INDEX_7));
}
#endif

//*****************************************************************************
//
//! \index
//! Convert PWM index to TCU ID.
//!
//! \param ulPWMIndex is the index of the PWM.
//!
//! This function converts PWM index to TCU ID.
//!
//! \return Returns a TCU ID correspones to a PWM.
//
//*****************************************************************************
static unsigned long
PWMIndexToTCUID(unsigned long ulPWMIndex)
{
    //
    // Check parameters
    //
    ASSERT(PWMIndexValid(ulBase));

    switch (ulPWMIndex) {
    case PWM_INDEX_7:
        return(TCU_TIMER7);
    case PWM_INDEX_6:
        return(TCU_TIMER6);
    case PWM_INDEX_5:
        return(TCU_TIMER5);
    case PWM_INDEX_4:
        return(TCU_TIMER4);
    case PWM_INDEX_3:
        return(TCU_TIMER3);
    case PWM_INDEX_2:
        return(TCU_TIMER2);
    case PWM_INDEX_1:
        return(TCU_TIMER1);
    case PWM_INDEX_0:
        return(TCU_TIMER0);
    default:
        ASSERT(false);
        return(-1);
    }
}

//*****************************************************************************
//
//! Enable PWM output function of a PWM(a TCU timer).
//!
//! \param ulBase is the base address of the PWM.
//! \param ulPWMIndex is the index of the PWM.
//!
//! This function enable PWM output function.
//!
//! \return Returns true if enable PWM operation success, false others.
//
//*****************************************************************************
tBoolean
PWMEnable(unsigned long ulBase, unsigned long ulPWMIndex)
{
    //
    // Check parameters
    //
    ASSERT(PWMBaseValid(ulBase));
    ASSERT(PWMIndexValid(ulPWMIndex));

    return(TCUCounterEnable(ulBase, PWMIndexToTCUID(ulPWMIndex)));
}

//*****************************************************************************
//
//! Disable PWM output function of a PWM(a TCU timer).
//!
//! \param ulBase is the base address of the PWM.
//! \param ulPWMIndex is the index of the PWM.
//!
//! This function disable PWM output function.
//!
//! \return Returns true if enable PWM operation success, false others.
//
//*****************************************************************************
tBoolean
PWMDisable(unsigned long ulBase, unsigned long ulPWMIndex)
{
    //
    // Check parameters
    //
    ASSERT(PWMBaseValid(ulBase));
    ASSERT(PWMIndexValid(ulPWMIndex));

    return(TCUCounterDisable(ulBase, PWMIndexToTCUID(ulPWMIndex)));
}

//*****************************************************************************
//
//! Configure PWM.
//!
//! \param ulBase is the base address of the PWM.
//! \param ulPWMIndex is the index of the PWM.
//! \param ulDuty is the duty(half) part of PWM pulse.
//! \param ulPeriod is the period(full) part of PWM pulse.
//!
//! This function configure PWM works in normal way(non FIFO mode).
//!
//! \return Returns true if configure PWM operation success, false others.
//
//*****************************************************************************
tBoolean
PWMConfigure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long ulDuty, unsigned long ulPeriod)
{
    unsigned long ulTimerID;

    //
    // Check parameters
    //
    ASSERT(PWMBaseValid(ulBase));
    ASSERT(PWMIndexValid(ulPWMIndex));

    // GPIO PE0 configure gpio PE0 as PWM function pin
    HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= 0x00000001;
    HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= 0x00000001;
    HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= 0x00000001;
    HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= 0x00000001;
    HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = 0x4;

    //
    ulTimerID = PWMIndexToTCUID(ulPWMIndex);
    if (ulTimerID == -1) {
        return(false);
    }

    // 1. Initial the configuration.
    // 1a. Writing TCSR.INITL to initialize PWM output level
    TCUPWMInitialOutputLevelSet(ulBase, ulTimerID, TCU_PWMINITOUTPUT_LOW);

    // 1b. Writing TCSR.SD to setting the shutdown mode(abrupt or graceful mode)
    TCUPWMShutdown(ulBase, ulTimerID, TCU_SHUTDOWN_GRACEFUL);

    // 1c. Writing TCSR.PRESCALE to set TCNT count clock frequency
    TCUClockInputPrescaleSet(ulBase, ulTimerID, TCU_CLOCKPRESCALE_4);

    // 1d. Setting TCNT, TDHR and TDFR
    TCUCounterSet(ulBase, ulTimerID, 0x0000);
    TCUDataHalfSet(ulBase, ulTimerID, ulDuty);
    TCUDataFullSet(ulBase, ulTimerID, ulPeriod);

    // 1e. Set Non FIFO mode
    if ((ulTimerID == TCU_TIMER5) ||
        (ulTimerID == TCU_TIMER4) ||
        (ulTimerID == TCU_TIMER3) ||
        (ulTimerID == TCU_TIMER0)) {
        TCUModeSet(ulBase, ulTimerID, TCU_MODE_NONFIFO);
    }

    // 2. Enable the clock
    // 2a. Writing TCSR.PWM_EN to set whether enable PWM
    TCUPWMEnable(ulBase, ulTimerID);

    // 2b. Writing TCSR.EXT_EN, TCSR.RTC_EN or TCSR.PCK_EN to 1 to select the input clock and enable the input clock.
    TCUClockInputSet(ulBase, ulTimerID, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(ulBase, ulTimerID);

    // 3. Enable the counter
    // This step is done in PWMEnable() function
    return(true);
}

//*****************************************************************************
//
//! Configure PWM (in FIFO mode 1).
//!
//! \param ulBase is the base address of the PWM.
//! \param ulPWMIndex is the index of the PWM.
//! \param pulDuty is the pointer of duty(half) array.
//! \param pulPeriod is the pointer of period(full) array.
//! \param ulFIFONumber is the size of array(both pulDuty and pulPeriod)
//!
//! This function configure PWM work in FIFO mode 1.
//! The content in pulDuty array and pulPeriod array can't contain zero 
//! TODO: needs to test carefully before use this feature
//!
//! \return Returns true if configure PWM operation success, false others.
//
//*****************************************************************************
tBoolean
PWMFIFOMode1Configure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long *pulDuty, unsigned long *pulPeriod, unsigned long ulFIFONumber)
{
    unsigned long ulTimerID;
    unsigned long i;
    // Used pwm fifo mode 1 more than once
    static tBoolean bFirstTime = true;

    //
    // Check parameters
    //
    ASSERT(PWMBaseValid(ulBase));
    ASSERT(PWMIndexValid(ulPWMIndex));
    ASSERT(ulFIFONumber <= TCU_FIFO_LENGTH);

    for (i = 0; i < ulFIFONumber; i++) {
        if ( (*(pulDuty + i) == 0) ||
             (*(pulPeriod + i) == 0)) {
            return(false);
        }
    }

    //
    ulTimerID = PWMIndexToTCUID(ulPWMIndex);
    if (ulTimerID == -1) {
        return(false);
    } else {
        if(!((ulTimerID == TCU_TIMER5) ||
             (ulTimerID == TCU_TIMER4) ||
             (ulTimerID == TCU_TIMER3) ||
             (ulTimerID == TCU_TIMER0))) {
            ASSERT(false);
            return(false);
        }
    }

    // Not first time run, add 'do it again' flow
    if (bFirstTime != true) {
        // 8. when fifo match finished ,if you want to do it again, please stop the TCNT count
        PWMDisable(ulBase, ulTimerID);

        // 9. set TCSR,please keep PWM_EN to 0
        TCUPWMDisable(ulBase, ulTimerID);

        // 10. set TFCR to clr the flag
        TCUComparisonMatchFlagClear(ulBase, ulTimerID, TCU_FLAGTYPE_FIFOEMPTY);

        // 11. set TCUMOD.fifo_clr to 1
        // 12. set TCUMOD.fifo_clr to 0
        TCUFIFOClear(ulBase, ulTimerID);

        // goto step 2 for continue
    } else {
        bFirstTime = false;

        // GPIO PE0 configure gpio PE0 as PWM function pin
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = 0x4;

        // 1. set TCSR, please keep PWM_EN to 0.
        TCUPWMDisable(ulBase, ulTimerID);
    }

    // 2. set TCUMOD to 0x0000_0001
    TCUModeSet(ulBase, ulTimerID, TCU_MODE_FIFO);

    // 3. push data into fifo, through write data in TFWD. fifo's depth is 16, do not over flow
    //    do not push data like 0x0000_XXXX, or 0xXXXX_0000 X: nonzero value
    for (i = 0; i < ulFIFONumber; i++) {
        unsigned long ulHalf = *(pulDuty + i);
        unsigned long ulFull = *(pulPeriod + i);
        unsigned long ulData = ((ulFull & 0xFFFF) << 16) | (ulHalf & 0xFFFF);
        TCUFIFOWrite(ulBase, ulTimerID, ulData);
    }

    // 4. set the TCSR.PWM_EN to 1
    TCUPWMEnable(ulBase, ulTimerID);

    // 5. set the TESR to enable the TCNT
    TCUCounterEnable(ulBase, ulTimerID);

    // 5a. set clock prescale and clock input
    TCUClockInputPrescaleSet(ulBase, ulTimerID, TCU_CLOCKPRESCALE_1024);
    TCUClockInputSet(ulBase, ulTimerID, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(ulBase, ulTimerID);

    // 6. push some data into fifo is optional. Please insure the TFIFOSR. fifo_w_flag is 0, before push
    //    data into fifo. and do not over flow. do not push data like 0x0000_XXXX, or 0xXXXX_0000 X: nonzero value
    // Here we do nothing

    // 7. when TFR.fifo_empty_flag is 1, symbol the fifo is empty
    // Here we do nothing

    return(true);
}

//*****************************************************************************
//
//! Configure PWM (in FIFO mode 2).
//!
//! \param ulBase is the base address of the PWM.
//! \param ulPWMIndex is the index of the PWM.
//! \param pulDuty is the pointer of duty(half) array.
//! \param pulPeriod is the pointer of period(full) array.
//! \param ulFIFONumber is the size of array(both pulDuty and pulPeriod)
//! \param ulFIFOCycleNumber is the cycle number that FIFO data will be used ulFIFOCycleNumber times
//!
//! This function configure PWM work in FIFO mode 2.
//! The content in pulDuty array and pulPeriod array can't contain zero 
//! TODO: needs to test carefully before use this feature
//!
//! \return Returns true if configure PWM operation success, false others.
//
//*****************************************************************************
tBoolean
PWMFIFOMode2Configure(unsigned long ulBase, unsigned long ulPWMIndex, unsigned long *pulDuty, unsigned long *pulPeriod, unsigned long ulFIFONumber, unsigned long ulFIFOCycleNumber)
{
    unsigned long ulTimerID;
    unsigned long i;
    // Used pwm fifo mode 2 more than once
    static tBoolean bFirstTime = true;

    //
    // Check parameters
    //
    ASSERT(PWMBaseValid(ulBase));
    ASSERT(PWMIndexValid(ulPWMIndex));
    ASSERT(ulFIFONumber <= TCU_FIFO_LENGTH);

    for (i = 0; i < ulFIFONumber; i++) {
        if ( (*(pulDuty + i) == 0) ||
             (*(pulPeriod + i) == 0)) {
            ASSERT(false);
            return(false);
        }
    }

    //
    ulTimerID = PWMIndexToTCUID(ulPWMIndex);
    if (ulTimerID == -1) {
        return(false);
    } else {
        if(!((ulTimerID == TCU_TIMER5) ||
             (ulTimerID == TCU_TIMER4) ||
             (ulTimerID == TCU_TIMER3) ||
             (ulTimerID == TCU_TIMER0))) {
            ASSERT(false);
            return(false);
        }
    }

    // Not first time run, add 'do it again' flow
    if (bFirstTime != true) {
        // 9. if you want do again, please stop the TCNT count
        PWMDisable(ulBase, ulTimerID);

        // 10. set the TCSR, Please keep PWM_EN to 0
        TCUPWMDisable(ulBase, ulTimerID);

        // 11. set TFCR to clr the flag
        TCUComparisonMatchFlagClear(ulBase, ulTimerID, TCU_FLAGTYPE_FIFOEMPTY);

        // 12. set TCUMOD.fifo_clr to 1
        // 13. set TCUMOD.fifo_clr to 0
        TCUFIFOClear(ulBase, ulTimerID);

        // 14. reset TCU_MODE0_CYCST/TCU_MODE0_FIFONUMST, must be done
        //     or the again flow not works, since TCNT not count any more
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_CYCST;
        HWREG(ulBase + TCU_O_MODE0) &= ~TCU_MODE0_FIFONUMST;

        // goto step 2 for continue
    } else {
        bFirstTime = false;

        // GPIO PE0 configure gpio PE0 as PWM function pin
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= 0x00000001;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = 0x4;

        // 1. set the TCSR, Please keep PWM_EN to 0
        TCUPWMDisable(ulBase, ulTimerID);

        // 1a. Writing TCSR.INITL to initialize PWM output level
        TCUPWMInitialOutputLevelSet(ulBase, ulTimerID, TCU_PWMINITOUTPUT_LOW);

        // 1b. Writing TCSR.SD to setting the shutdown mode(abrupt or graceful mode)
        TCUPWMShutdown(ulBase, ulTimerID, TCU_SHUTDOWN_GRACEFUL);
    }

    // 2. set TCUMOD to 0x0000_0001.
    TCUModeSet(ulBase, ulTimerID, TCU_MODE_FIFO);

    // 3. push data into fifo, through write data in TFWD. fifo`s depth is 16, do not over flow
    //    do not push data like 0x0000_XXXX, or 0xXXXX_0000 X: nonzero value
    for (i = 0; i < ulFIFONumber; i++) {
        unsigned long ulHalf = *(pulDuty + i);
        unsigned long ulFull = *(pulPeriod + i);
        unsigned long ulData = ((ulFull & 0xFFFF) << 16) | (ulHalf & 0xFFFF);
        TCUFIFOWrite(ulBase, ulTimerID, ulData);
    }
    
    // 4. set TCUMOD
    TCUFIFOModeSet(ulBase, ulTimerID, TCU_FIFOMODE_FIFO2);
    TCUFIFOCycleNumberSet(ulBase, ulTimerID, ulFIFOCycleNumber);
    TCUFIFONumberSet(ulBase, ulTimerID, ulFIFONumber);

    // 5. set the TCSR.PWM_EN to 1.
    TCUPWMEnable(ulBase, ulTimerID);

    // 5a. set clock prescale and clock input
    TCUClockInputPrescaleSet(ulBase, ulTimerID, TCU_CLOCKPRESCALE_1024);
    TCUClockInputSet(ulBase, ulTimerID, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(ulBase, ulTimerID);

    // 6. set the TESR to enable the TCNT
    TCUCounterEnable(ulBase, ulTimerID);

    // 7. if step 4 has been done, do not push data into fifo
    // Do nothing here

    // 8. when TFIFOSR. cyc_timer == TCUMOD. cyc_num , It symbol finished.
    // Do nothing here

    return(true);
}
