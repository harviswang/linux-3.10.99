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
static unsigned long
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
