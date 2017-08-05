//*****************************************************************************
//
// cpm.c - Driver for the CPM(Clock Power Management).
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

#include "../inc/hw_cpm.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "debug.h"
#include "cpm.h"
#include "intc.h"
#include "clock.h"

//*****************************************************************************
//
//! \internal
//! Checks a CPM base address.
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function determines if a CPM base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
CPMBaseValid(unsigned long ulBase)
{   
    return(ulBase == CPM_BASE);
}
#endif

//*****************************************************************************
//
//! Check if APLL is by passed
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function check the bypass status of APLL.
//!
//! \return Returns \b true APLL is bypassed and \b false otherwise
//
//*****************************************************************************
tBoolean
CPMApllIsBypassed(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    return((HWREG(ulBase + CPM_O_PCR) & CPM_PCR_APLLBP) == CPM_PCR_APLLBP);
}

//*****************************************************************************
//
//! Get multiplication of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 12-bit multiplication factor of APLL.
//!
//! \return Returns APLL's multiplication factor
//
//*****************************************************************************
unsigned long
CPMApllGetM(unsigned long ulBase)
{
    unsigned long ulM;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulM = (HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLM) >> 20;
    return(ulM);
}

//*****************************************************************************
//
//! Set multiplication of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 12-bit multiplication factor of APLL.
//!
//! \return Returns true if APLL's multiplication factor set operation is ok
//
//*****************************************************************************
tBoolean
CPMApllSetM(unsigned long ulBase, unsigned long ulM)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulM = ulM << 20;
    HWREG(ulBase + CPM_O_APCR) = ulM | (HWREG(ulBase + CPM_O_APCR) & (~CPM_APCR_APLLM));
    return((HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLM) == ulM);
}

//*****************************************************************************
//
//! Get divider of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 6-bit divider factor of APLL.
//!
//! \return Returns APLL's divider factor
//
//*****************************************************************************
unsigned long
CPMApllGetN(unsigned long ulBase)
{
    unsigned long ulN;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulN = (HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLN) >> 14;
    return(ulN);
}

//*****************************************************************************
//
//! Set divider of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 6-bit divider factor of APLL.
//!
//! \return Returns true if APLL's divider factor set operation is ok
//
//*****************************************************************************
tBoolean
CPMApllSetN(unsigned long ulBase, unsigned long ulN)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulN = ulN << 14;
    HWREG(ulBase + CPM_O_APCR) = ulN | (HWREG(ulBase + CPM_O_APCR) & (~CPM_APCR_APLLN));
    return((HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLN) == ulN);
}

//*****************************************************************************
//
//! Get divider fraction of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 24-bit divider fraction of APLL.
//!
//! \return Returns APLL's divider fraction 
//
//*****************************************************************************
unsigned long
CPMApllGetNFraction(unsigned long ulBase)
{
    unsigned long ulNFraction;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulNFraction = (HWREG(ulBase + CPM_O_APACR) & CPM_APACR_FRAC);
    return(ulNFraction);
}

//*****************************************************************************
//
//! Set divider fraction of APLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 24-bit divider fraction of APLL.
//!
//! \return Returns true if APLL's divider fraction set operation is successed.
//
//*****************************************************************************
tBoolean
CPMApllSetNFraction(unsigned long ulBase, unsigned long ulNFraction)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    HWREG(ulBase + CPM_O_APACR) = ulNFraction;
    return( (HWREG(ulBase + CPM_O_APACR) & CPM_APACR_FRAC) == ulNFraction);
}

//*****************************************************************************
//
//! APLL enable
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function enable APLL.
//!
//! \return Returns true if APLL's enable operation is success, other false
//
//*****************************************************************************
tBoolean
CPMApllEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase))

    HWREG(ulBase + CPM_O_APCR) |= CPM_APCR_APLLEN;
    return((HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLON) == CPM_APCR_APLLON);
}

//*****************************************************************************
//
//! APLL disable
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function disable APLL.
//!
//! \return Returns true if APLL's disable operation is success, other false
//
//*****************************************************************************
tBoolean
CPMApllDisable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase))

    HWREG(ulBase + CPM_O_APCR) &= ~CPM_APCR_APLLEN;
    return((HWREG(ulBase + CPM_O_APCR) & CPM_APCR_APLLON) != CPM_APCR_APLLON);
}

//*****************************************************************************
//
//! Get multiplication of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 12-bit multiplication factor of MPLL.
//!
//! \return Returns MPLL's multiplication factor
//
//*****************************************************************************
unsigned long
CPMMpllGetM(unsigned long ulBase)
{
    unsigned long ulM;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulM = (HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLM) >> 20;
    return(ulM);
}

//*****************************************************************************
//
//! Set multiplication of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 12-bit multiplication factor of MPLL.
//!
//! \return Returns true if MPLL's multiplication factor set operation is ok.
//
//*****************************************************************************
tBoolean
CPMMpllSetM(unsigned long ulBase, unsigned long ulM)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulM = ulM << 20;
    HWREG(ulBase + CPM_O_MPCR) = ulM | (HWREG(ulBase + CPM_O_MPCR) & (~CPM_MPCR_MPLLM));
    return((HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLM) == ulM);
}

//*****************************************************************************
//
//! Get divider of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 6-bit divider factor of MPLL.
//!
//! \return Returns MPLL's divider factor
//
//*****************************************************************************
unsigned long
CPMMpllGetN(unsigned long ulBase)
{
    unsigned long ulN;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulN = (HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLN) >> 14;
    return(ulN);
}

//*****************************************************************************
//
//! Set divider of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 6-bit divider factor of MPLL.
//!
//! \return Returns true if MPLL's divider factor set operation successed.
//
//*****************************************************************************
tBoolean
CPMMpllSetN(unsigned long ulBase, unsigned long ulN)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulN = ulN << 14;
    HWREG(ulBase + CPM_O_MPCR) = ulN | (HWREG(ulBase + CPM_O_MPCR) & (~CPM_MPCR_MPLLN));
    return((HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLN) == ulN);
}

//*****************************************************************************
//
//! Get divider fraction of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function get the 24-bit divider fraction of MPLL.
//!
//! \return Returns MPLL's divider fraction 
//
//*****************************************************************************
unsigned long
CPMMpllGetNFraction(unsigned long ulBase)
{
    unsigned long ulNFraction;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    ulNFraction = (HWREG(ulBase + CPM_O_MPACR) & CPM_MPACR_FRAC);
    return(ulNFraction);
}

//*****************************************************************************
//
//! Set divider fraction of MPLL
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function set the 24-bit divider fraction of MPLL.
//!
//! \return Returns true if MPLL's divider fraction set operation is successed.
//
//*****************************************************************************
tBoolean
CPMMpllSetNFraction(unsigned long ulBase, unsigned long ulNFraction)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    HWREG(ulBase + CPM_O_MPACR) = ulNFraction;
    return((HWREG(ulBase + CPM_O_MPACR) & CPM_MPACR_FRAC) == ulNFraction);
}

//*****************************************************************************
//
//! \internal
//! Get greatest common dividor of two integer
//!
//! \param ulA the first integer.
//! \param ulB the second integer, can not be zero.
//!
//! This function get the greatest common dividor of ulA and ulB.
//!
//! \return Returns GCD(ulA, ulB) 
//
//*****************************************************************************
unsigned long
CPMGreatestCommonDividor(unsigned long ulA, unsigned long ulB)
{
    unsigned long ulC;

    //
    // Check the arguments.
    //
    ASSERT(ulB != 0);

    ulC = ulA % ulB;
    if (ulC == 0) {
        return ulB;
    } else {
        return CPMGreatestCommonDividor(ulB, ulC);
    }
}

//*****************************************************************************
//
//! MPLL enable
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function enable MPLL.
//!
//! \return Returns true if MPLL's enable operation is success, other false
//
//*****************************************************************************
tBoolean
CPMMpllEnable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase))

    HWREG(ulBase + CPM_O_MPCR) |= CPM_MPCR_MPLLEN;
    return((HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLON) == CPM_MPCR_MPLLON);
}

//*****************************************************************************
//
//! MPLL disable
//!
//! \param ulBase is the base address of the CPM.
//!
//! This function disable MPLL.
//!
//! \return Returns true if MPLL's disable operation is success, other false
//
//*****************************************************************************
tBoolean
CPMMpllDisable(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase))

    HWREG(ulBase + CPM_O_MPCR) &= ~CPM_MPCR_MPLLEN;
    return((HWREG(ulBase + CPM_O_MPCR) & CPM_MPCR_MPLLON) != CPM_MPCR_MPLLON);
}

//*****************************************************************************
//
//! Calculate PLL's multiplication factor M, multiplication fraction part MFraction
//! and dividor factor N dividor fraction part NFraction
//!
//! \param ulOutputFrequency the pll's output clock frequency.
//! \param ulReferenceFrequency the pll's reference clock frequency.
//! \param pulM memory to store multiplication factor M
//! \param pulMFraction memory to store multiplication fraction, bit[31:16] store fen mu
//!        bit[15:0] store fen zi, in SoC there is no register to store pulMFraction
//! \param pulN memory to store dividor factor N
//! \param pulNFraction memory to store dividor fraction, bit[31:16] store fen mu,
//!        bit[15:0] store fen zi, in SoC there is no register to store pulMFraction,
//!        this is best to be zero
//!
//! This function calculate M, N, Fraction for a pll frequency generator.
//!
//! \return Returns true always
//*****************************************************************************
tBoolean
CPMPLLCalculateMNFraction(unsigned long ulOutputFrequency, unsigned long ulReferenceFrequency,
unsigned long *pulM, unsigned long *pulMFraction, unsigned long *pulN, unsigned long *pulNFraction)
{
    unsigned long ulMMax, ulNMax;
    unsigned long ulGCD = CPMGreatestCommonDividor(ulOutputFrequency, ulReferenceFrequency);
    unsigned short *pusM, *pusN;

    //
    // Check the arguments.
    //
    ASSERT(ulReferenceFrequency != 0);
    ASSERT(pulM != NULL);
    ASSERT(pulMFraction != NULL);
    ASSERT(pulN != NULL);
    ASSERT(pulNFraction != NULL);

    pusM = (unsigned short *)pulMFraction;
    pusN = (unsigned short *)pulNFraction;

    *pulM = ulOutputFrequency / ulGCD;
    ulMMax = (1 << 12) - 1;

    *pulN = ulReferenceFrequency / ulGCD;
    ulNMax = (1 << 6) - 1;

    *pulMFraction = 0;
    *pulNFraction = 0;

    while (*pulN > ulNMax || *pulM > ulMMax) {
        unsigned long n = (*pulN + ulNMax - 1) / ulNMax;
        if ((*pulN % ulNMax) == 0) {
            *pulNFraction = 0;
        } else {
            *pusN = *pulN % n; /* fen zi part maybe zero */
            if (*pusN == 0) {  /* fen mu part never zero */
                *(pusN + 1) = 0;
            } else {
                *(pusN + 1) = n;
            }
        }
        *pulN = *pulN / n;

        if ((*pulM % n) == 0) {
            *pulMFraction = 0;
        } else {
            *pusM = *pulM % n; /* fen zi part maybe zero */
            if (*pusM == 0) {  /* fen mu part never zero */
                *(pusM + 1) = 0;
            } else {
                *(pusM + 1) = n;
            }
        }
        *pulM = *pulM / n;
    }

    return(true); 
}

//*****************************************************************************
//
//! Enable a clock(unset clock's gate)
//!
//! \param ulBase is the base address of the CPM.
//! \param ulClockNo the clock ID, defined in clock.h.
//!
//! This function unset clock's gate, so registers of the device can be accessed.
//!
//! \return Returns true if unset clock's gate operation is ok, false others
//
//*****************************************************************************
tBoolean
CPMClockEnable(unsigned long ulBase, unsigned long ulClockNo)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    switch (ulClockNo) {
    // Included in clock gate 0
    case CLOCK_NO_NFI:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_NFI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_NFI) != CPM_CLKGR0_NFI);
    case CLOCK_NO_NEMC:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_NEMC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_NEMC) != CPM_CLKGR0_NEMC);
    case CLOCK_NO_BCH:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_BCH;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_BCH) != CPM_CLKGR0_BCH);
    case CLOCK_NO_OTG:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_OTG;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_OTG) != CPM_CLKGR0_OTG);
    case CLOCK_NO_MSC0:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_MSC0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC0) != CPM_CLKGR0_MSC0);
    case CLOCK_NO_MSC1:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_MSC1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC1) != CPM_CLKGR0_MSC1);
    case CLOCK_NO_SSI0:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SSI0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI0) != CPM_CLKGR0_SSI0);
    case CLOCK_NO_SMB0:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SMB0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB0) != CPM_CLKGR0_SMB0);
    case CLOCK_NO_SMB1:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SMB1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB1) != CPM_CLKGR0_SMB1);
    case CLOCK_NO_SMB2:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SMB2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB2) != CPM_CLKGR0_SMB2);
    case CLOCK_NO_SMB3:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SMB3;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB3) != CPM_CLKGR0_SMB3);
    case CLOCK_NO_AIC:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_AIC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_AIC) != CPM_CLKGR0_AIC);
    case CLOCK_NO_MSC2:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_MSC2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC2) != CPM_CLKGR0_MSC2);
    case CLOCK_NO_SADC:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SADC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SADC) != CPM_CLKGR0_SADC);
    case CLOCK_NO_UART0:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UART0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART0) != CPM_CLKGR0_UART0);
    case CLOCK_NO_UART1:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UART1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART1) != CPM_CLKGR0_UART1);
    case CLOCK_NO_UART2:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UART2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART2) != CPM_CLKGR0_UART2);
    case CLOCK_NO_UART3:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UART3;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART3) != CPM_CLKGR0_UART3);
    case CLOCK_NO_UART4:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UART4;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART4) != CPM_CLKGR0_UART4);
    case CLOCK_NO_SSI1:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SSI1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI1) != CPM_CLKGR0_SSI1);
    case CLOCK_NO_SSI2:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_SSI2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI2) != CPM_CLKGR0_SSI2);
    case CLOCK_NO_PDMA:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_PDMA;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_PDMA) != CPM_CLKGR0_PDMA);
    case CLOCK_NO_UHC:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_UHC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UHC) != CPM_CLKGR0_UHC);
    case CLOCK_NO_ISP:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_ISP;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_ISP) != CPM_CLKGR0_ISP);
    case CLOCK_NO_LCD:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_LCD;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_LCD) != CPM_CLKGR0_LCD);
    case CLOCK_NO_CSI:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_CSI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_CSI) != CPM_CLKGR0_CSI);
    case CLOCK_NO_DSI:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_DSI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DSI) != CPM_CLKGR0_DSI);
    case CLOCK_NO_PCM:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_PCM;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_PCM) != CPM_CLKGR0_PCM);
    case CLOCK_NO_DES:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_DES;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DES) != CPM_CLKGR0_DES);
    case CLOCK_NO_RTC:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_RTC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_RTC) != CPM_CLKGR0_RTC);
    case CLOCK_NO_TCU:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_TCU;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_TCU) != CPM_CLKGR0_TCU);
    case CLOCK_NO_DDR:
        HWREG(ulBase + CPM_O_CLKGR0) &= ~CPM_CLKGR0_DDR;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DDR) != CPM_CLKGR0_DDR);
    // Included in clock gate 1
    case CLOCK_NO_VPU:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_VPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_VPU) != CPM_CLKGR1_VPU);
    case CLOCK_NO_GPU:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_GPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_GPU) != CPM_CLKGR1_GPU);
    case CLOCK_NO_IPU:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_IPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_IPU) != CPM_CLKGR1_IPU);
    case CLOCK_NO_AHBMON:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_AHBMON;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AHBMON) != CPM_CLKGR1_AHBMON);
    case CLOCK_NO_EPD:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_EPD;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_EPD) != CPM_CLKGR1_EPD);
    case CLOCK_NO_AES:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_AES;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AES) != CPM_CLKGR1_AES);
    case CLOCK_NO_HASH:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_HASH;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_HASH) != CPM_CLKGR1_HASH);
    case CLOCK_NO_DMIC:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_DMIC;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_DMIC) != CPM_CLKGR1_DMIC);
    case CLOCK_NO_P1:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_P1;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_P1) != CPM_CLKGR1_P1);
    case CLOCK_NO_P0:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_P0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_P0) != CPM_CLKGR1_P0);
    case CLOCK_NO_AHB0:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_AHB0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AHB0) != CPM_CLKGR1_AHB0);
    case CLOCK_NO_SYSOST:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_SYSOST;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_SYSOST) != CPM_CLKGR1_SYSOST);
    case CLOCK_NO_TCUEXCLK:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_TCUEXCLK;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_TCUEXCLK) != CPM_CLKGR1_TCUEXCLK);
    case CLOCK_NO_DLINE:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_DLINE;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_DLINE) != CPM_CLKGR1_DLINE);
    case CLOCK_NO_APB0:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_APB0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_APB0) != CPM_CLKGR1_APB0);
    case CLOCK_NO_CPU:
        HWREG(ulBase + CPM_O_CLKGR1) &= ~CPM_CLKGR1_CPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_CPU) != CPM_CLKGR1_CPU);
    // Others(not controlled in clock gate register)
    case CLOCK_NO_SCLKA:
        HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_GATESCLKA;
        return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_GATESCLKA) != CPM_CCR_GATESCLKA);
    case CLOCK_NO_I2S:
        // TODO
    case CLOCK_NO_CIM:
        // TODO
    case CLOCK_NO_APLL:
       return(CPMApllEnable(ulBase));
    case CLOCK_NO_MPLL:
       return(CPMMpllEnable(ulBase));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Disable a clock(set clock's gate)
//!
//! \param ulBase is the base address of the CPM.
//! \param ulClockNo the clock ID, defined in clock.h.
//!
//! This function set clock's gate, so registers of the device can't be accessed.
//!
//! \return Returns true if set clock's gate operation is ok, false others
//
//*****************************************************************************
tBoolean
CPMClockDisable(unsigned long ulBase, unsigned long ulClockNo)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    switch (ulClockNo) {
    // Included in clock gate 0
    case CLOCK_NO_NFI:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_NFI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_NFI) == CPM_CLKGR0_NFI);
    case CLOCK_NO_NEMC:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_NEMC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_NEMC) == CPM_CLKGR0_NEMC);
    case CLOCK_NO_BCH:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_BCH;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_BCH) == CPM_CLKGR0_BCH);
    case CLOCK_NO_OTG:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_OTG;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_OTG) == CPM_CLKGR0_OTG);
    case CLOCK_NO_MSC0:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_MSC0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC0) == CPM_CLKGR0_MSC0);
    case CLOCK_NO_MSC1:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_MSC1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC1) == CPM_CLKGR0_MSC1);
    case CLOCK_NO_SSI0:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SSI0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI0) == CPM_CLKGR0_SSI0);
    case CLOCK_NO_SMB0:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SMB0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB0) == CPM_CLKGR0_SMB0);
    case CLOCK_NO_SMB1:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SMB1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB1) == CPM_CLKGR0_SMB1);
    case CLOCK_NO_SMB2:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SMB2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB2) == CPM_CLKGR0_SMB2);
    case CLOCK_NO_SMB3:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SMB3;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SMB3) == CPM_CLKGR0_SMB3);
    case CLOCK_NO_AIC:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_AIC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_AIC) == CPM_CLKGR0_AIC);
    case CLOCK_NO_MSC2:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_MSC2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_MSC2) == CPM_CLKGR0_MSC2);
    case CLOCK_NO_SADC:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SADC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SADC) == CPM_CLKGR0_SADC);
    case CLOCK_NO_UART0:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UART0;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART0) == CPM_CLKGR0_UART0);
    case CLOCK_NO_UART1:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UART1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART1) == CPM_CLKGR0_UART1);
    case CLOCK_NO_UART2:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UART2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART2) == CPM_CLKGR0_UART2);
    case CLOCK_NO_UART3:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UART3;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART3) == CPM_CLKGR0_UART3);
    case CLOCK_NO_UART4:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UART4;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UART4) == CPM_CLKGR0_UART4);
    case CLOCK_NO_SSI1:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SSI1;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI1) == CPM_CLKGR0_SSI1);
    case CLOCK_NO_SSI2:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_SSI2;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_SSI2) == CPM_CLKGR0_SSI2);
    case CLOCK_NO_PDMA:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_PDMA;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_PDMA) == CPM_CLKGR0_PDMA);
    case CLOCK_NO_UHC:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_UHC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_UHC) == CPM_CLKGR0_UHC);
    case CLOCK_NO_ISP:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_ISP;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_ISP) == CPM_CLKGR0_ISP);
    case CLOCK_NO_LCD:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_LCD;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_LCD) == CPM_CLKGR0_LCD);
    case CLOCK_NO_CSI:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_CSI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_CSI) == CPM_CLKGR0_CSI);
    case CLOCK_NO_DSI:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_DSI;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DSI) == CPM_CLKGR0_DSI);
    case CLOCK_NO_PCM:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_PCM;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_PCM) == CPM_CLKGR0_PCM);
    case CLOCK_NO_DES:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_DES;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DES) == CPM_CLKGR0_DES);
    case CLOCK_NO_RTC:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_RTC;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_RTC) == CPM_CLKGR0_RTC);
    case CLOCK_NO_TCU:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_TCU;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_TCU) == CPM_CLKGR0_TCU);
    case CLOCK_NO_DDR:
        HWREG(ulBase + CPM_O_CLKGR0) |= CPM_CLKGR0_DDR;
        return((HWREG(ulBase + CPM_O_CLKGR0) & CPM_CLKGR0_DDR) == CPM_CLKGR0_DDR);
    // Included in clock gate 1
    case CLOCK_NO_VPU:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_VPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_VPU) == CPM_CLKGR1_VPU);
    case CLOCK_NO_GPU:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_GPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_GPU) == CPM_CLKGR1_GPU);
    case CLOCK_NO_IPU:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_IPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_IPU) == CPM_CLKGR1_IPU);
    case CLOCK_NO_AHBMON:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_AHBMON;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AHBMON) == CPM_CLKGR1_AHBMON);
    case CLOCK_NO_EPD:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_EPD;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_EPD) == CPM_CLKGR1_EPD);
    case CLOCK_NO_AES:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_AES;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AES) == CPM_CLKGR1_AES);
    case CLOCK_NO_HASH:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_HASH;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_HASH) == CPM_CLKGR1_HASH);
    case CLOCK_NO_DMIC:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_DMIC;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_DMIC) == CPM_CLKGR1_DMIC);
    case CLOCK_NO_P1:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_P1;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_P1) == CPM_CLKGR1_P1);
    case CLOCK_NO_P0:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_P0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_P0) == CPM_CLKGR1_P0);
    case CLOCK_NO_AHB0:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_AHB0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_AHB0) == CPM_CLKGR1_AHB0);
    case CLOCK_NO_SYSOST:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_SYSOST;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_SYSOST) == CPM_CLKGR1_SYSOST);
    case CLOCK_NO_TCUEXCLK:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_TCUEXCLK;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_TCUEXCLK) == CPM_CLKGR1_TCUEXCLK);
    case CLOCK_NO_DLINE:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_DLINE;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_DLINE) == CPM_CLKGR1_DLINE);
    case CLOCK_NO_APB0:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_APB0;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_APB0) == CPM_CLKGR1_APB0);
    case CLOCK_NO_CPU:
        HWREG(ulBase + CPM_O_CLKGR1) |= CPM_CLKGR1_CPU;
        return((HWREG(ulBase + CPM_O_CLKGR1) & CPM_CLKGR1_CPU) == CPM_CLKGR1_CPU);
    // Others(not controlled in clock gate register)
    case CLOCK_NO_SCLKA:
        HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_GATESCLKA;
        return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_GATESCLKA) == CPM_CCR_GATESCLKA);
    case CLOCK_NO_I2S:
        // TODO
    case CLOCK_NO_CIM:
        // TODO
    case CLOCK_NO_APLL:
        return(CPMApllDisable(ulBase));
    case CLOCK_NO_MPLL:
        return(CPMMpllDisable(ulBase));
    default:
        ASSERT(false);
        return(false);
    }
}

//*****************************************************************************
//
//! Get divider of a clock.
//!
//! \param ulBase is the base address of the CPM.
//! \param ulClockNo is the clock id number defined in file clock.h
//!
//! This function get the divider value of a clock.
//!
//! \return Returns the clock's divider, if not support return 0
//
//*****************************************************************************
unsigned long
CPMClockDividerGet(unsigned long ulBase, unsigned long ulClockNo)
{
    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    switch (ulClockNo) {
    // Included in clock gate 0
    case CLOCK_NO_NFI:
        break;
    case CLOCK_NO_NEMC:
        break;
    case CLOCK_NO_BCH:
        return(1 + (HWREG(ulBase + CPM_O_BCHCDR) & CPM_BCHCDR_BCHCDR));
    case CLOCK_NO_OTG:
        break;
    case CLOCK_NO_MSC0:
        return((1 + (HWREG(ulBase + CPM_O_MSC0CDR) & CPM_MSC0CDR_MSC0CDR)) * 2);
    case CLOCK_NO_MSC1:
        return((1 + (HWREG(ulBase + CPM_O_MSC1CDR) & CPM_MSC1CDR_MSC1CDR)) * 2);
    case CLOCK_NO_SSI0:
        return(1 + (HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR));
    case CLOCK_NO_SMB0:
        break;
    case CLOCK_NO_SMB1:
        break;
    case CLOCK_NO_SMB2:
        break;
    case CLOCK_NO_SMB3:
        break;
    case CLOCK_NO_AIC:
        break;
    case CLOCK_NO_MSC2:
        return((1 + (HWREG(ulBase + CPM_O_MSC2CDR) & CPM_MSC2CDR_MSC2CDR)) * 2);
    case CLOCK_NO_SADC:
        break;
    case CLOCK_NO_UART0:
        break;
    case CLOCK_NO_UART1:
        break;
    case CLOCK_NO_UART2:
        break;
    case CLOCK_NO_UART3:
        break;
    case CLOCK_NO_UART4:
        break;
    case CLOCK_NO_SSI1:
        return(1 + (HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR));
        break;
    case CLOCK_NO_SSI2:
        return(1 + (HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR));
        break;
    case CLOCK_NO_PDMA:
        break;
    case CLOCK_NO_UHC:
        return(1 + (HWREG(ulBase + CPM_O_UHCCDR) & CPM_UHCCDR_UHCCDR));
    case CLOCK_NO_ISP:
        return(1 + (HWREG(ulBase + CPM_O_ISPCDR) & CPM_ISPCDR_ISPCDR));
    case CLOCK_NO_LCD:
        return(1 + (HWREG(ulBase + CPM_O_LCDPCDR) & CPM_LCDPCDR_LCDPCDR));
    case CLOCK_NO_CSI:
        break;
    case CLOCK_NO_DSI:
        break;
    case CLOCK_NO_PCM:
        return(1 + (HWREG(ulBase + CPM_O_PCMCDR) & CPM_PCMCDR_PCMCDR));
    case CLOCK_NO_DES:
        break;
    case CLOCK_NO_RTC:
        break;
    case CLOCK_NO_TCU:
        break;
    case CLOCK_NO_DDR:
        return(1 + (HWREG(ulBase + CPM_O_DDRCDR) & CPM_DDRCDR_DDRCDR));
    // Included in clock gate 1
    case CLOCK_NO_VPU:
        return(1 + (HWREG(ulBase + CPM_O_VPUCDR) & CPM_VPUCDR_VPUCDR));
    case CLOCK_NO_GPU:
        return(1 + (HWREG(ulBase + CPM_O_GPUCDR) & CPM_GPUCDR_GPUCDR));
    case CLOCK_NO_IPU:
        break;
    case CLOCK_NO_AHBMON:
        break;
    case CLOCK_NO_EPD:
        break;
    case CLOCK_NO_AES:
        break;
    case CLOCK_NO_HASH:
        break;
    case CLOCK_NO_DMIC:
        break;
    case CLOCK_NO_P1:
        break;
    case CLOCK_NO_P0:
        return(1 + (HWREG(ulBase + CPM_O_CCR) & CPM_CCR_CDIV));
    case CLOCK_NO_AHB0:
        return(1 + ((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_H0DIV) >> 8));
    case CLOCK_NO_SYSOST:
        break;
    case CLOCK_NO_TCUEXCLK:
        break;
    case CLOCK_NO_DLINE:
        break;
    case CLOCK_NO_APB0:
        break;
    case CLOCK_NO_CPU:
        return(1 + (HWREG(ulBase + CPM_O_CCR) & CPM_CCR_CDIV));
    // Others(not controlled in clock gate register)
    case CLOCK_NO_SCLKA:
        break;
    case CLOCK_NO_I2S:
        return(1 + (HWREG(ulBase + CPM_O_I2SCDR) & CPM_I2SCDR_I2SCDR));
    case CLOCK_NO_CIM:
        return(1 + (HWREG(ulBase + CPM_O_CIMCDR) & CPM_CIMCDR_CIMCDR));
    case CLOCK_NO_APLL:
        break;
    case CLOCK_NO_MPLL:
        break;
    case CLOCK_NO_L2CACHE:
        return(1 + ((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_L2CDIV) >> 4));
    case CLOCK_NO_PCLK:
        return(1 + ((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_PDIV) >> 16));
    case CLOCK_NO_AHB2:
        return(1 + ((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_H2DIV) >> 12));
    default:
        break;
    }

    return(0);
}

//*****************************************************************************
//
//! Set divider of a clock.
//!
//! \param ulBase is the base address of the CPM.
//! \param ulClockNo is the clock id number defined in file clock.h
//! \param ulClockDivider is the divider value
//!
//! This function set the divider value of a clock.
//!
//! \return Returns true if the clock's divider set operation is ok.
//
//*****************************************************************************
tBoolean
CPMClockDividerSet(unsigned long ulBase, unsigned long ulClockNo, unsigned long ulClockDivider)
{
    unsigned long ulCpuP0Divider;
    unsigned long ulL2CacheDivider;
    unsigned long ulPeripheralDivider;
    unsigned long ulAHB2Divider;

    //
    // Check the arguments.
    //
    ASSERT(UARTBaseValid(ulBase));

    switch (ulClockNo) {
    // Included in clock gate 0
    case CLOCK_NO_NFI:
        break;
    case CLOCK_NO_NEMC:
        break;
    case CLOCK_NO_BCH:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_BCHCDR) |= CPM_BCHCDR_CEBCH;
        HWREG(ulBase + CPM_O_BCHCDR) = (HWREG(ulBase + CPM_O_BCHCDR) & (~CPM_BCHCDR_BCHCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_BCHCDR) &= ~CPM_BCHCDR_CEBCH;
        return((HWREG(ulBase + CPM_O_BCHCDR) & CPM_BCHCDR_BCHCDR) == ulClockDivider);
    case CLOCK_NO_OTG:
        break;
    case CLOCK_NO_MSC0: // if ulClockDivider is odd number, the accuracy is not as you desired
        ulClockDivider >>= 1;
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_MSC0CDR) |= CPM_MSC0CDR_CEMSC0;
        HWREG(ulBase + CPM_O_MSC0CDR) = (HWREG(ulBase + CPM_O_MSC0CDR) & (~CPM_MSC0CDR_MSC0CDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_MSC0CDR) &= ~CPM_MSC0CDR_CEMSC0;
        return((HWREG(ulBase + CPM_O_MSC0CDR) & CPM_MSC0CDR_MSC0CDR) == ulClockDivider);
    case CLOCK_NO_MSC1:
        ulClockDivider >>= 1;
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_MSC1CDR) |= CPM_MSC1CDR_CEMSC1;
        HWREG(ulBase + CPM_O_MSC1CDR) = (HWREG(ulBase + CPM_O_MSC1CDR) & (~CPM_MSC1CDR_MSC1CDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_MSC1CDR) &= ~CPM_MSC1CDR_CEMSC1;
        return((HWREG(ulBase + CPM_O_MSC1CDR) & CPM_MSC1CDR_MSC1CDR) == ulClockDivider);
    case CLOCK_NO_SSI0:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_SSICDR) |= CPM_SSICDR_CESSI;
        HWREG(ulBase + CPM_O_SSICDR) = (HWREG(ulBase + CPM_O_SSICDR) & (~CPM_SSICDR_SSICDR)) | ulClockDivider;
        return((HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR) == ulClockDivider);
    case CLOCK_NO_SMB0:
        break;
    case CLOCK_NO_SMB1:
        break;
    case CLOCK_NO_SMB2:
        break;
    case CLOCK_NO_SMB3:
        break;
    case CLOCK_NO_AIC:
        break;
    case CLOCK_NO_MSC2:
        ulClockDivider >>= 1;
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_MSC2CDR) |= CPM_MSC2CDR_CEMSC2;
        HWREG(ulBase + CPM_O_MSC2CDR) = (HWREG(ulBase + CPM_O_MSC2CDR) & (~CPM_MSC2CDR_MSC2CDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_MSC2CDR) &= ~CPM_MSC2CDR_CEMSC2;
        return((HWREG(ulBase + CPM_O_MSC2CDR) & CPM_MSC2CDR_MSC2CDR) == ulClockDivider);
    case CLOCK_NO_SADC:
        break;
    case CLOCK_NO_UART0:
        break;
    case CLOCK_NO_UART1:
        break;
    case CLOCK_NO_UART2:
        break;
    case CLOCK_NO_UART3:
        break;
    case CLOCK_NO_UART4:
        break;
    case CLOCK_NO_SSI1:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_SSICDR) |= CPM_SSICDR_CESSI;
        HWREG(ulBase + CPM_O_SSICDR) = (HWREG(ulBase + CPM_O_SSICDR) & (~CPM_SSICDR_SSICDR)) | ulClockDivider;
        return((HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR) == ulClockDivider);
    case CLOCK_NO_SSI2:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_SSICDR) |= CPM_SSICDR_CESSI;
        HWREG(ulBase + CPM_O_SSICDR) = (HWREG(ulBase + CPM_O_SSICDR) & (~CPM_SSICDR_SSICDR)) | ulClockDivider;
        return((HWREG(ulBase + CPM_O_SSICDR) & CPM_SSICDR_SSICDR) == ulClockDivider);
    case CLOCK_NO_PDMA:
        break;
    case CLOCK_NO_UHC:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_UHCCDR) |= CPM_UHCCDR_CEUHC;
        HWREG(ulBase + CPM_O_UHCCDR) = (HWREG(ulBase + CPM_O_UHCCDR) & (~CPM_UHCCDR_UHCCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_UHCCDR) &= ~CPM_UHCCDR_CEUHC;
        return((HWREG(ulBase + CPM_O_UHCCDR) & CPM_UHCCDR_UHCCDR) == ulClockDivider);
    case CLOCK_NO_ISP:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_ISPCDR) |= CPM_ISPCDR_CEISP;
        HWREG(ulBase + CPM_O_ISPCDR) = (HWREG(ulBase + CPM_O_ISPCDR) & (~CPM_ISPCDR_ISPCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_ISPCDR) &= ~CPM_ISPCDR_CEISP;
        return((HWREG(ulBase + CPM_O_ISPCDR) & CPM_ISPCDR_ISPCDR) == ulClockDivider);
    case CLOCK_NO_LCD:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_LCDPCDR) |= CPM_LCDPCDR_CELCD;
        HWREG(ulBase + CPM_O_LCDPCDR) = (HWREG(ulBase + CPM_O_LCDPCDR) & (~CPM_LCDPCDR_LCDPCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_LCDPCDR) &= ~CPM_LCDPCDR_CELCD;
        return((HWREG(ulBase + CPM_O_LCDPCDR) & CPM_LCDPCDR_LCDPCDR) == ulClockDivider);
    case CLOCK_NO_CSI:
        ulClockDivider -= 1;
        break;
    case CLOCK_NO_DSI:
        break;
    case CLOCK_NO_PCM:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_PCMCDR) |= CPM_PCMCDR_CEPCM;
        HWREG(ulBase + CPM_O_PCMCDR) = (HWREG(ulBase + CPM_O_PCMCDR) & (~CPM_PCMCDR_PCMCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_PCMCDR) &= ~CPM_PCMCDR_CEPCM;
        return((HWREG(ulBase + CPM_O_PCMCDR) & CPM_PCMCDR_PCMCDR) == ulClockDivider);
    case CLOCK_NO_DES:
        break;
    case CLOCK_NO_RTC:
        break;
    case CLOCK_NO_TCU:
        break;
    case CLOCK_NO_DDR:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_DDRCDR) |= CPM_DDRCDR_CEDDR;
        HWREG(ulBase + CPM_O_DDRCDR) = (HWREG(ulBase + CPM_O_DDRCDR) & (~CPM_DDRCDR_DDRCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_DDRCDR) &= ~CPM_DDRCDR_CEDDR;
        return((HWREG(ulBase + CPM_O_DDRCDR) & CPM_DDRCDR_DDRCDR) == ulClockDivider);
    // Included in clock gate 1
    case CLOCK_NO_VPU:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_VPUCDR) |= CPM_VPUCDR_CEVPU;
        HWREG(ulBase + CPM_O_VPUCDR) = (HWREG(ulBase + CPM_O_VPUCDR) & (~CPM_VPUCDR_VPUCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_VPUCDR) &= ~CPM_VPUCDR_CEVPU;
        return((HWREG(ulBase + CPM_O_VPUCDR) & CPM_VPUCDR_VPUCDR) == ulClockDivider);
    case CLOCK_NO_GPU:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_GPUCDR) |= CPM_GPUCDR_CEGPU;
        HWREG(ulBase + CPM_O_GPUCDR) = (HWREG(ulBase + CPM_O_GPUCDR) & (~CPM_GPUCDR_GPUCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_GPUCDR) &= ~CPM_GPUCDR_CEGPU;
        return(HWREG(ulBase + CPM_O_GPUCDR) & CPM_GPUCDR_GPUCDR);
    case CLOCK_NO_IPU:
        break;
    case CLOCK_NO_AHBMON:
        break;
    case CLOCK_NO_EPD:
        break;
    case CLOCK_NO_AES:
        break;
    case CLOCK_NO_HASH:
        break;
    case CLOCK_NO_DMIC:
        break;
    case CLOCK_NO_P1:
        break;
    case CLOCK_NO_P0: // must be changed with L2Cache frequency together, make sure freq(L2Cache) = 1/2 * freq(cpu_p0)
                      // when sclk_a is 1032000000, I tested cpu p0 206400000/344000000/516000000/1032000000 works 
        //CACHE_PREFETCH(LAB, 64);
        //FAST_IOB();
//LAB:
        ulL2CacheDivider = CPMClockDividerGet(ulBase, CLOCK_NO_L2CACHE);
        if ((ulL2CacheDivider == ulClockDivider * 2) ||
            (ulL2CacheDivider == ulClockDivider * 3) ||
            (ulL2CacheDivider == ulClockDivider * 4))
        {
            // No need to change the frequency of L2Cache
        } else {
            ulL2CacheDivider = ulClockDivider << 1;
            //ulL2CacheDivider = ulClockDivider * 3; // ok
            //ulL2CacheDivider = ulClockDivider * 4; // not always ok
        }
        ulL2CacheDivider -= 1;
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_CECPU;
        HWREG(ulBase + CPM_O_CCR) = (HWREG(ulBase + CPM_O_CCR) & (~CPM_CCR_CDIV) & (~CPM_CCR_L2CDIV)) | ulClockDivider | (ulL2CacheDivider << 4);
        while ((HWREG(ulBase + CPM_O_CSR) & CPM_CSR_CDIVBUSY) == CPM_CSR_CDIVBUSY) {
            continue; // wait until frequency change finished
        }
        HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_CECPU;
        return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_CDIV) == ulClockDivider);
    case CLOCK_NO_AHB0:
        ulClockDivider -= 1;
        ulClockDivider <<= 8;
        HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_CEAHB0;
        HWREG(ulBase + CPM_O_CCR) = (HWREG(ulBase + CPM_O_CCR) & (~CPM_CCR_H0DIV)) | ulClockDivider;
        HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_CEAHB0;
        return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_H0DIV) == ulClockDivider);
    case CLOCK_NO_SYSOST:
        break;
    case CLOCK_NO_TCUEXCLK:
        break;
    case CLOCK_NO_DLINE:
        break;
    case CLOCK_NO_APB0:
        break;
    case CLOCK_NO_CPU:
        break;
    // Others(not controlled in clock gate register)
    case CLOCK_NO_SCLKA:
        break;
    case CLOCK_NO_I2S:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_I2SCDR) |= CPM_I2SCDR_CEI2S;
        HWREG(ulBase + CPM_O_I2SCDR) = (HWREG(ulBase + CPM_O_I2SCDR) & (~CPM_I2SCDR_I2SCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_I2SCDR) &= ~CPM_I2SCDR_CEI2S;
        return((HWREG(ulBase + CPM_O_I2SCDR) & CPM_I2SCDR_I2SCDR) == ulClockDivider);
    case CLOCK_NO_CIM:
        ulClockDivider -= 1;
        HWREG(ulBase + CPM_O_CIMCDR) |= CPM_CIMCDR_CECIM;
        HWREG(ulBase + CPM_O_CIMCDR) = (HWREG(ulBase + CPM_O_CIMCDR) & (~CPM_CIMCDR_CIMCDR)) | ulClockDivider;
        HWREG(ulBase + CPM_O_CIMCDR) &= ~CPM_CIMCDR_CECIM;
        return((HWREG(ulBase + CPM_O_CIMCDR) & CPM_CIMCDR_CIMCDR) == ulClockDivider);
    case CLOCK_NO_APLL:
        break;
    case CLOCK_NO_MPLL:
        break;
    case CLOCK_NO_L2CACHE: // should be 1/1 or 1/2 or 1/3 or 1/4 of cpu_clk, others not work(1/1 also not works in my test)
                           // when sclk_a is 1032000000, I tested l2cache 86000000/103200000/114666666/129000000/172000000/258000000/344000000/516000000
        ulCpuP0Divider = CPMClockDividerGet(ulBase, CLOCK_NO_P0);
        if ((ulCpuP0Divider * 2 == ulClockDivider) ||
            (ulCpuP0Divider * 3 == ulClockDivider) ||
            (ulCpuP0Divider * 4 == ulClockDivider))
        {
            ulClockDivider -= 1;
            ulClockDivider <<= 4;
            HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_CECPU;
            HWREG(ulBase + CPM_O_CCR) = (HWREG(ulBase + CPM_O_CCR) & (~CPM_CCR_L2CDIV)) | ulClockDivider;
            HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_CECPU;
            return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_L2CDIV) == ulClockDivider);
        } else {
            break;
        }
    case CLOCK_NO_PCLK:
        ulAHB2Divider = CPMClockDividerGet(ulBase, CLOCK_NO_AHB2);
        if ((ulAHB2Divider == ulClockDivider) || (ulClockDivider * 2 == ulClockDivider)) {
            ulClockDivider -= 1;
            ulClockDivider <<= 16;
            HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_CEAHB2;
            HWREG(ulBase + CPM_O_CCR) = (HWREG(ulBase + CPM_O_CCR) & (~CPM_CCR_PDIV)) | ulClockDivider;
            HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_CEAHB2;
            return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_PDIV) == ulClockDivider);
        } else {
            break;
        }
    case CLOCK_NO_AHB2: // freq(AHB2) should be 1 or 2 times of freq(PCLK)
        ulPeripheralDivider = CPMClockDividerGet(ulBase, CLOCK_NO_PCLK);
        if ((ulPeripheralDivider == ulClockDivider) || (ulPeripheralDivider == ulClockDivider * 2)) {
            ulClockDivider -= 1;
            ulClockDivider <<= 12;
            HWREG(ulBase + CPM_O_CCR) |= CPM_CCR_CEAHB2;
            HWREG(ulBase + CPM_O_CCR) = (HWREG(ulBase + CPM_O_CCR) & (~CPM_CCR_H2DIV)) | ulClockDivider;
            HWREG(ulBase + CPM_O_CCR) &= ~CPM_CCR_CEAHB2;
            return((HWREG(ulBase + CPM_O_CCR) & CPM_CCR_H2DIV) == ulClockDivider);
        } else {
            break;
        }
    default:
        break;
    }

    return(false);
}
