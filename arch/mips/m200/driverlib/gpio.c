//*****************************************************************************
//
// gpio.c - Driver for the GPIO.
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

#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "gpio.h"
#include "intc.h"
#include "debug.h"

//*****************************************************************************
//
// A mapping of GPIO base address to interupt number.
//
//*****************************************************************************
static const unsigned long g_ppulGPIOIntMap[][2] =
{
    { GPIOA_BASE, IRQ_NO_GPIO0 },
    { GPIOB_BASE, IRQ_NO_GPIO1 },
    { GPIOC_BASE, IRQ_NO_GPIO2 },
    { GPIOD_BASE, IRQ_NO_GPIO3 },
    { GPIOE_BASE, IRQ_NO_GPIO4 },
    { GPIOF_BASE, IRQ_NO_GPIO5 },
};

//*****************************************************************************
//
//! \internal
//! Checks a GPIO base address.
//!
//! \param ulBase is the base address of the GPIO port.
//!
//! This function determines if a GPIO port base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
GPIOBaseValid(unsigned long ulBase)
{
    return((ulBase == GPIOA_BASE) ||
           (ulBase == GPIOB_BASE) ||
           (ulBase == GPIOC_BASE) ||
           (ulBase == GPIOD_BASE) ||
           (ulBase == GPIOE_BASE) ||
           (ulBase == GPIOF_BASE));
}
#endif

//*****************************************************************************
//
//! \internal
//! Checks a GPIO pin number.
//!
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function determines if a GPIO pin number is valid, [0, 31].
//!
//! \return Returns \b true if the pin number is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
tBoolean
GPIOPinNumberValid(unsigned long ulPinNumber)
{
    return(ulPinNumber <= 31 );
}
#endif

//*****************************************************************************
//
//! \internal
//!  Get GPIO group id.
//!
//! \param ulBase is the base of the GPIO port.
//!
//! This function gets the group id of GPIO port.
//!
//! \return Returns group id[0, 6] if success, return -1 ([7, 0xF]) when failed
//
//*****************************************************************************
static long
GPIOGroupIDGet(unsigned long ulBase)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));

    ulGroupID = (ulBase >> 8) & 0xF;
    if (ulGroupID < 7) {
        return(ulGroupID);
    } else {
        return(-1);
    }
}

//*****************************************************************************
//
//! Disable(Mask) a GPIO interrupt.
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function mask a gpio interrupt.
//!
//! \return Returns \b true if mask operation is success and \b false
//! otherwise.
//
//*****************************************************************************
tBoolean
GPIOInterruptDisable(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0);
    case 1:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1);
    case 2:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2);
    case 3:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3);
    case 4:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4);
    case 5:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5);
    case 6:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6);
    case 7:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7);
    case 8:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8);
    case 9:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9);
    case 10:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10);
    case 11:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11);
    case 12:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12);
    case 13:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13);
    case 14:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14);
    case 15:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15);
    case 16:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16);
    case 17:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17);
    case 18:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18);
    case 19:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19);
    case 20:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20);
    case 21:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21);
    case 22:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22);
    case 23:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23);
    case 24:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24);
    case 25:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25);
    case 26:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26);
    case 27:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27);
    case 28:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28);
    case 29:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29);
    case 30:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30);
    case 31:
        HWREG(ulBase + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31);
    default:
        break;
    }

    return(false);
}

//*****************************************************************************
//
//! Enable(Unmask) a GPIO interrupt.
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function clear gpio interrupt mask.
//!
//! \return Returns \b true if the unmask operation is success and \b false
//! otherwise.
//
//*****************************************************************************
tBoolean
GPIOInterruptEnable(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0);
    case 1:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1);
    case 2:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2);
    case 3:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3);
    case 4:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4);
    case 5:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5);
    case 6:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6);
    case 7:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7);
    case 8:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8);
    case 9:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9);
    case 10:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10);
    case 11:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11);
    case 12:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12);
    case 13:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13);
    case 14:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14);
    case 15:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15);
    case 16:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16);
    case 17:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17);
    case 18:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18);
    case 19:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19);
    case 20:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20);
    case 21:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21);
    case 22:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22);
    case 23:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23);
    case 24:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24);
    case 25:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25);
    case 26:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26);
    case 27:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27);
    case 28:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28);
    case 29:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29);
    case 30:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30);
    case 31:
        HWREG(ulBase + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        return((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31);
    default:
        break;
    }

    return(false);
}

//*****************************************************************************
//
//! Clear a GPIO interrupt.
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function clear gpio interrupt.
//!
//! \return Returns \b true if the clear operation is success and \b false
//! otherwise.
//
//*****************************************************************************
tBoolean
GPIOInterruptClear(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC0;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG0) != GPIO_FLAG_FLAG0);
    case 1:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC1;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG1) != GPIO_FLAG_FLAG1);
    case 2:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC2;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG2) != GPIO_FLAG_FLAG2);
    case 3:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC3;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG3) != GPIO_FLAG_FLAG3);
    case 4:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC4;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG4) != GPIO_FLAG_FLAG4);
    case 5:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC5;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG5) != GPIO_FLAG_FLAG5);
    case 6:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC6;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG6) != GPIO_FLAG_FLAG6);
    case 7:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC7;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG7) != GPIO_FLAG_FLAG7);
    case 8:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC8;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG8) != GPIO_FLAG_FLAG8);
    case 9:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC9;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG9) != GPIO_FLAG_FLAG9);
    case 10:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC10;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG10) != GPIO_FLAG_FLAG10);
    case 11:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC11;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG11) != GPIO_FLAG_FLAG11);
    case 12:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC12;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG12) != GPIO_FLAG_FLAG12);
    case 13:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC13;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG13) != GPIO_FLAG_FLAG13);
    case 14:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC14;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG14) != GPIO_FLAG_FLAG14);
    case 15:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC15;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG15) != GPIO_FLAG_FLAG15);
    case 16:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC16;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG16) != GPIO_FLAG_FLAG16);
    case 17:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC17;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG17) != GPIO_FLAG_FLAG17);
    case 18:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC18;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG18) != GPIO_FLAG_FLAG18);
    case 19:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC19;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG19) != GPIO_FLAG_FLAG19);
    case 20:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC20;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG20) != GPIO_FLAG_FLAG20);
    case 21:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC21;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG21) != GPIO_FLAG_FLAG21);
    case 22:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC22;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG22) != GPIO_FLAG_FLAG22);
    case 23:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC23;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG23) != GPIO_FLAG_FLAG23);
    case 24:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC24;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG24) != GPIO_FLAG_FLAG24);
    case 25:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC25;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG25) != GPIO_FLAG_FLAG25);
    case 26:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC26;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG26) != GPIO_FLAG_FLAG26);
    case 27:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC27;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG27) != GPIO_FLAG_FLAG27);
    case 28:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC28;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG28) != GPIO_FLAG_FLAG28);
    case 29:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC29;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG29) != GPIO_FLAG_FLAG29);
    case 30:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC30;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG30) != GPIO_FLAG_FLAG30);
    case 31:
        HWREG(ulBase + GPIO_O_FLAGC) |= GPIO_FLAGC_FLAGC31;
        return((HWREG(ulBase + GPIO_O_FLAG) & GPIO_FLAG_FLAG31) != GPIO_FLAG_FLAG31);
    default:
        break;
    }

    return(false);
}

//*****************************************************************************
//
//! Set GPIO interrupt type.
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//! \parma ulInterruptType is the interrupt type of the GPIO pin
//!
//! This function set gpio interrupt type.
//!
//! \return Returns \b true if the clear operation is success and \b false
//! otherwise.
//
//*****************************************************************************
tBoolean
GPIOInterruptTypeSet(unsigned long ulBase, unsigned long ulPinNumber, unsigned long ulInterruptType)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));
    ASSERT((ulInterruptType == GPIO_FUNCTION_RAISEINT) ||
           (ulInterruptType == GPIO_FUNCTION_FALLINT)  ||
           (ulInterruptType == GPIO_FUNCTION_HIGHINT)  ||
           (ulInterruptType == GPIO_FUNCTION_LOWINT));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    return(GPIOFunctionSet(ulBase, ulPinNumber, ulInterruptType));
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(raise interrupt function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio raise interrupt function type.
//!
//! \return Returns \b true if the raise interrupt function set operation is success and \b false
//! otherwise.
//
//*****************************************************************************
static tBoolean
GPIORaiseInterruptFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {

    default:
        return(false);
    }
}


//*****************************************************************************
//
//! \internal
//! Set GPIO function(falling interrupt function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio raise interrupt function type.
//!
//! \return Returns \b true if the raise interrupt function set operation is success and \b false
//! otherwise.
//
//*****************************************************************************
static tBoolean
GPIOFallingInterruptFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(high level triggered interrupt function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio high level interrupt function type.
//!
//! \return Returns \b true if the high level interrupt function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOHighInterruptFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(low level triggered interrupt function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio low level interrupt function type.
//!
//! \return Returns \b true if the low level interrupt function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOLowInterruptFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(normal input function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio input function type.
//!
//! \return Returns \b true if the input function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOInputFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(output high function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio output high function type.
//!
//! \return Returns \b true if the output high function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOOutputHighFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(output low function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio output low function type.
//!
//! \return Returns \b true if the output low function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOOuptLowFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(device 3 function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio device 3 function type.
//!
//! \return Returns \b true if the device 3 function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIODevice3FunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(device 2 function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio device 2 function type.
//!
//! \return Returns \b true if the device 2 function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIODevice2FunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(device 1 function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio device 1 function type.
//!
//! \return Returns \b true if the device 1 function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIODevice1FunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(device 0 function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set gpio device 0 function type.
//!
//! \return Returns \b true if the device 0 function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIODevice0FunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   != GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) != GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   != GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) != GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   != GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) != GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   != GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) != GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   != GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) != GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   != GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) != GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   != GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) != GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   != GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) != GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   != GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) != GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   != GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) != GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   != GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) != GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   != GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) != GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   != GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) != GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   != GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) != GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   != GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) != GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   != GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) != GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   != GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) != GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   != GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) != GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   != GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) != GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   != GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) != GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   != GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) != GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   != GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) != GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   != GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) != GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   != GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) != GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   != GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) != GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   != GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) != GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   != GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) != GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   != GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) != GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   != GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) != GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   != GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) != GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   != GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) != GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTC)  |= GPIO_INTC_INTC31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKC) |= GPIO_MASKC_MASKC31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   != GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) != GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(rising edge interrupt mask function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set rising edge mask interrupt function type.
//!
//! \return Returns \b true if the rising edge mask function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIORaiseInterruptMaskFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(falling edge interrupt mask function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set falling edge mask interrupt function type.
//!
//! \return Returns \b true if the falling edge mask function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOFallingInterruptMaskFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) == GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) == GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) == GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) == GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) == GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) == GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) == GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) == GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) == GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) == GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) == GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) == GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) == GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) == GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) == GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) == GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) == GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) == GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) == GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) == GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) == GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) == GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) == GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) == GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) == GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) == GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) == GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) == GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) == GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) == GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) == GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1S) |= GPIO_PAT1S_PAT1S31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) == GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(high interrupt mask function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set high interrupt mask function type.
//!
//! \return Returns \b true if the high interrupt mask function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOHighInterruptMaskFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) == GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) == GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) == GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) == GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) == GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) == GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) == GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) == GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) == GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) == GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) == GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) == GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) == GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) == GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) == GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) == GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) == GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) == GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) == GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) == GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) == GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) == GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) == GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) == GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) == GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) == GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) == GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) == GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) == GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) == GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) == GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0S) |= GPIO_PAT0S_PAT0S31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) == GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! \internal
//! Set GPIO function(low interrupt mask function).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function set low interrupt mask function type.
//!
//! \return Returns \b true if the low interrupt mask function set operation
//! is success and \b false otherwise.
//
//*****************************************************************************
static tBoolean
GPIOLowInterruptMaskFunctionSet(unsigned long ulBase, unsigned long ulPinNumber)
{
    unsigned long ulGroupID;

    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    ulGroupID = GPIOGroupIDGet(ulBase);
    ASSERT(ulGroupID != -1);

    switch (ulPinNumber) {
    case 0:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS0;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C0;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C0;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT0)   == GPIO_INT_INT0)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK0) == GPIO_MASK_MASK0) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT10) != GPIO_PAT1_PAT10) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT00) != GPIO_PAT0_PAT00));
    case 1:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS1;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C1;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C1;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT1)   == GPIO_INT_INT1)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK1) == GPIO_MASK_MASK1) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT11) != GPIO_PAT1_PAT11) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT01) != GPIO_PAT0_PAT01));
    case 2:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS2;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C2;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C2;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT2)   == GPIO_INT_INT2)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK2) == GPIO_MASK_MASK2) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT12) != GPIO_PAT1_PAT12) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT02) != GPIO_PAT0_PAT02));
    case 3:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS3;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C3;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C3;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT3)   == GPIO_INT_INT3)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK3) == GPIO_MASK_MASK3) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT13) != GPIO_PAT1_PAT13) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT03) != GPIO_PAT0_PAT03));
    case 4:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS4;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C4;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C4;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT4)   == GPIO_INT_INT4)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK4) == GPIO_MASK_MASK4) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT14) != GPIO_PAT1_PAT14) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT04) != GPIO_PAT0_PAT04));
    case 5:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS5;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C5;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C5;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT5)   == GPIO_INT_INT5)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK5) == GPIO_MASK_MASK5) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT15) != GPIO_PAT1_PAT15) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT05) != GPIO_PAT0_PAT05));
    case 6:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS6;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C6;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C6;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT6)   == GPIO_INT_INT6)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK6) == GPIO_MASK_MASK6) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT16) != GPIO_PAT1_PAT16) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT06) != GPIO_PAT0_PAT06));
    case 7:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS7;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C7;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C7;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT7)   == GPIO_INT_INT7)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK7) == GPIO_MASK_MASK7) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT17) != GPIO_PAT1_PAT17) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT07) != GPIO_PAT0_PAT07));
    case 8:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS8;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C8;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C8;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT8)   == GPIO_INT_INT8)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK8) == GPIO_MASK_MASK8) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT18) != GPIO_PAT1_PAT18) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT08) != GPIO_PAT0_PAT08));
    case 9:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS9;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C9;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C9;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT9)   == GPIO_INT_INT9)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK9) == GPIO_MASK_MASK9) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT19) != GPIO_PAT1_PAT19) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT09) != GPIO_PAT0_PAT09));
    case 10:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS10;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C10;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C10;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT10)   == GPIO_INT_INT10)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK10) == GPIO_MASK_MASK10) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT110) != GPIO_PAT1_PAT110) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT010) != GPIO_PAT0_PAT010));
    case 11:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS11;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C11;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C11;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT11)   == GPIO_INT_INT11)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK11) == GPIO_MASK_MASK11) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT111) != GPIO_PAT1_PAT111) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT011) != GPIO_PAT0_PAT011));
    case 12:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS12;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C12;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C12;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT12)   == GPIO_INT_INT12)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK12) == GPIO_MASK_MASK12) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT112) != GPIO_PAT1_PAT112) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT012) != GPIO_PAT0_PAT012));
    case 13:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS13;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C13;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C13;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT13)   == GPIO_INT_INT13)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK13) == GPIO_MASK_MASK13) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT113) != GPIO_PAT1_PAT113) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT013) != GPIO_PAT0_PAT013));
    case 14:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS14;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C14;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C14;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT14)   == GPIO_INT_INT14)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK14) == GPIO_MASK_MASK14) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT114) != GPIO_PAT1_PAT114) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT014) != GPIO_PAT0_PAT014));
    case 15:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS15;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C15;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C15;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT15)   == GPIO_INT_INT15)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK15) == GPIO_MASK_MASK15) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT115) != GPIO_PAT1_PAT115) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT015) != GPIO_PAT0_PAT015));
    case 16:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS16;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C16;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C16;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT16)   == GPIO_INT_INT16)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK16) == GPIO_MASK_MASK16) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT116) != GPIO_PAT1_PAT116) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT016) != GPIO_PAT0_PAT016));
    case 17:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS17;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C17;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C17;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT17)   == GPIO_INT_INT17)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK17) == GPIO_MASK_MASK17) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT117) != GPIO_PAT1_PAT117) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT017) != GPIO_PAT0_PAT017));
    case 18:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS18;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C18;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C18;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT18)   == GPIO_INT_INT18)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK18) == GPIO_MASK_MASK18) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT118) != GPIO_PAT1_PAT118) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT018) != GPIO_PAT0_PAT018));
    case 19:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS19;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C19;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C19;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT19)   == GPIO_INT_INT19)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK19) == GPIO_MASK_MASK19) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT119) != GPIO_PAT1_PAT119) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT019) != GPIO_PAT0_PAT019));
    case 20:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS20;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C20;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C20;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT20)   == GPIO_INT_INT20)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK20) == GPIO_MASK_MASK20) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT120) != GPIO_PAT1_PAT120) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT020) != GPIO_PAT0_PAT020));
    case 21:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS21;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C21;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C21;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT21)   == GPIO_INT_INT21)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK21) == GPIO_MASK_MASK21) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT121) != GPIO_PAT1_PAT121) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT021) != GPIO_PAT0_PAT021));
    case 22:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS22;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C22;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C22;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT22)   == GPIO_INT_INT22)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK22) == GPIO_MASK_MASK22) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT122) != GPIO_PAT1_PAT122) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT022) != GPIO_PAT0_PAT022));
    case 23:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS23;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C23;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C23;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT23)   == GPIO_INT_INT23)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK23) == GPIO_MASK_MASK23) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT123) != GPIO_PAT1_PAT123) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT023) != GPIO_PAT0_PAT023));
    case 24:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS24;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C24;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C24;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT24)   == GPIO_INT_INT24)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK24) == GPIO_MASK_MASK24) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT124) != GPIO_PAT1_PAT124) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT024) != GPIO_PAT0_PAT024));
    case 25:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS25;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C25;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C25;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT25)   == GPIO_INT_INT25)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK25) == GPIO_MASK_MASK25) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT125) != GPIO_PAT1_PAT125) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT025) != GPIO_PAT0_PAT025));
    case 26:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS26;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C26;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C26;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT26)   == GPIO_INT_INT26)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK26) == GPIO_MASK_MASK26) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT126) != GPIO_PAT1_PAT126) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT026) != GPIO_PAT0_PAT026));
    case 27:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS27;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C27;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C27;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT27)   == GPIO_INT_INT27)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK27) == GPIO_MASK_MASK27) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT127) != GPIO_PAT1_PAT127) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT027) != GPIO_PAT0_PAT027));
    case 28:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS28;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C28;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C28;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT28)   == GPIO_INT_INT28)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK28) == GPIO_MASK_MASK28) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT128) != GPIO_PAT1_PAT128) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT028) != GPIO_PAT0_PAT028));
    case 29:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS29;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C29;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C29;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT29)   == GPIO_INT_INT29)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK29) == GPIO_MASK_MASK29) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT129) != GPIO_PAT1_PAT129) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT029) != GPIO_PAT0_PAT029));
    case 30:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS30;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C30;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C30;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT30)   == GPIO_INT_INT30)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK30) == GPIO_MASK_MASK30) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT130) != GPIO_PAT1_PAT130) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT030) != GPIO_PAT0_PAT030));
    case 31:
        HWREG(GPIOZ_BASE + GPIO_O_INTS)  |= GPIO_INTS_INTS31;
        HWREG(GPIOZ_BASE + GPIO_O_MASKS) |= GPIO_MASKS_MASKS31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT1C) |= GPIO_PAT1C_PAT1C31;
        HWREG(GPIOZ_BASE + GPIO_O_PAT0C) |= GPIO_PAT0C_PAT0C31;
        HWREG(GPIOZ_BASE + GPIO_O_PGZIDLD) = ulGroupID;

        return(((HWREG(ulBase + GPIO_O_INT)  & GPIO_INT_INT31)   == GPIO_INT_INT31)   &&
               ((HWREG(ulBase + GPIO_O_MASK) & GPIO_MASK_MASK31) == GPIO_MASK_MASK31) &&
               ((HWREG(ulBase + GPIO_O_PAT1) & GPIO_PAT1_PAT131) != GPIO_PAT1_PAT131) &&
               ((HWREG(ulBase + GPIO_O_PAT0) & GPIO_PAT0_PAT031) != GPIO_PAT0_PAT031));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! Set GPIO function.
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//! \parma ulFunction is the function type of the GPIO pin
//!
//! This function set gpio function type.
//!
//! \return Returns \b true if the function set operation is success and \b false
//! otherwise.
//
//*****************************************************************************
tBoolean
GPIOFunctionSet(unsigned long ulBase, unsigned long ulPinNumber, unsigned long ulFunction)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));
    ASSERT((ulFunction == GPIO_FUNCTION_RAISEINT) ||
           (ulFunction == GPIO_FUNCTION_FALLINT)  ||
           (ulFunction == GPIO_FUNCTION_HIGHINT)  ||
           (ulFunction == GPIO_FUNCTION_LOWINT)   ||

           (ulFunction == GPIO_FUNCTION_INTPUT)   ||

           (ulFunction == GPIO_FUNCTION_OUTPUT1)  ||
           (ulFunction == GPIO_FUNCTION_OUTPUT0)  ||

           (ulFunction == GPIO_FUNCTION_DEVICE3)  ||
           (ulFunction == GPIO_FUNCTION_DEVICE2)  ||
           (ulFunction == GPIO_FUNCTION_DEVICE1)  ||
           (ulFunction == GPIO_FUNCTION_DEVICE0)  ||

           (ulFunction == GPIO_FUNCTION_RAISEINTM)||
           (ulFunction == GPIO_FUNCTION_FALLINTM) ||
           (ulFunction == GPIO_FUNCTION_HIGHINTM) ||
           (ulFunction == GPIO_FUNCTION_LOWINTM));

    switch(ulFunction) {
    case GPIO_FUNCTION_RAISEINT:
        return(GPIORaiseInterruptFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_FALLINT:
        return(GPIOFallingInterruptFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_HIGHINT:
        return(GPIOHighInterruptFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_LOWINT:
        return(GPIOLowInterruptFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_INTPUT:
        return(GPIOInputFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_OUTPUT1:
        return(GPIOOutputHighFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_OUTPUT0:
        return(GPIOOuptLowFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_DEVICE3:
        return(GPIODevice3FunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_DEVICE2:
        return(GPIODevice2FunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_DEVICE1:
        return(GPIODevice1FunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_DEVICE0:
        return(GPIODevice0FunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_RAISEINTM:
        return(GPIORaiseInterruptMaskFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_FALLINTM:
        return(GPIOFallingInterruptMaskFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_HIGHINTM:
        return(GPIOHighInterruptMaskFunctionSet(ulBase, ulPinNumber));
    case GPIO_FUNCTION_LOWINTM:
        return(GPIOLowInterruptMaskFunctionSet(ulBase, ulPinNumber));
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! Get GPIO number base(the first gpio number in a gpio port).
//!
//! \param ulBase is the base address of the GPIO port.
//!
//! This function get gpio base number.
//!
//! \return Returns gpio base number
//
//*****************************************************************************
unsigned long
GPIONumberBaseGet(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));

    switch (ulBase) {
    case GPIOA_BASE:
        return(0);
    case GPIOB_BASE:
        return(32);
    case GPIOC_BASE:
        return(64);
    case GPIOD_BASE:
        return(96);
    case GPIOE_BASE:
        return(128);
    case GPIOF_BASE:
        return(160);
    default:
        return(-1);
    }
}

//*****************************************************************************
//
//! Get GPIO base by gpio base number(revert of GPIONumberBaseGet).
//!
//! \param ulNumberBase is the base address of the GPIO port.
//!
//! This function get gpio memory base by gpio number number.
//!
//! \return Returns gpio memory base
//
//*****************************************************************************
unsigned long
GPIOBaseGet(unsigned long ulNumberBase)
{
    //
    // Check the arguments.
    //
    ASSERT((ulNumberBase == 0)   ||
           (ulNumberBase == 32)  ||
           (ulNumberBase == 64)  ||
           (ulNumberBase == 96)  ||
           (ulNumberBase == 128) ||
           (ulNumberBase == 160));

    switch (ulNumberBase) {
    case 0:
        return(GPIOA_BASE);
    case 32:
        return(GPIOB_BASE);
    case 64:
        return(GPIOC_BASE);
    case 96:
        return(GPIOD_BASE);
    case 128:
        return(GPIOE_BASE);
    case 160:
        return(GPIOF_BASE);
    default:
        return(-1);
    }
}

//*****************************************************************************
//
//! Get GPIO number base(the first gpio number in a gpio port).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function get gpio base number.
//!
//! \return Returns gpio base number
//
//*****************************************************************************
unsigned long
GPIOPinLevelGet(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL0) == GPIO_PIN_PNL0);
    case 1:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL1) == GPIO_PIN_PNL1);
    case 2:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL2) == GPIO_PIN_PNL2);
    case 3:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL3) == GPIO_PIN_PNL3);
    case 4:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL4) == GPIO_PIN_PNL4);
    case 5:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL5) == GPIO_PIN_PNL5);
    case 6:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL6) == GPIO_PIN_PNL6);
    case 7:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL7) == GPIO_PIN_PNL7);
    case 8:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL8) == GPIO_PIN_PNL8);
    case 9:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL9) == GPIO_PIN_PNL9);
    case 10:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL10) == GPIO_PIN_PNL10);
    case 11:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL11) == GPIO_PIN_PNL11);
    case 12:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL12) == GPIO_PIN_PNL12);
    case 13:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL13) == GPIO_PIN_PNL13);
    case 14:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL14) == GPIO_PIN_PNL14);
    case 15:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL15) == GPIO_PIN_PNL15);
    case 16:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL16) == GPIO_PIN_PNL16);
    case 17:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL17) == GPIO_PIN_PNL17);
    case 18:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL18) == GPIO_PIN_PNL18);
    case 19:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL19) == GPIO_PIN_PNL19);
    case 20:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL20) == GPIO_PIN_PNL20);
    case 21:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL21) == GPIO_PIN_PNL21);
    case 22:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL22) == GPIO_PIN_PNL22);
    case 23:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL23) == GPIO_PIN_PNL23);
    case 24:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL24) == GPIO_PIN_PNL24);
    case 25:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL25) == GPIO_PIN_PNL25);
    case 26:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL26) == GPIO_PIN_PNL26);
    case 27:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL27) == GPIO_PIN_PNL27);
    case 28:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL28) == GPIO_PIN_PNL28);
    case 29:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL29) == GPIO_PIN_PNL29);
    case 30:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL30) == GPIO_PIN_PNL30);
    case 31:
        return((HWREG(ulBase + GPIO_O_PIN) & GPIO_PIN_PNL31) == GPIO_PIN_PNL31);
    default:
        return(-1);
    }
}

//*****************************************************************************
//
//! Enable GPIO internal pull resistor(pull down or pull up is pin dependence).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function enable gpio internal pull register.
//!
//! \return Returns true if enable pull resistero operation success, others false
//
//*****************************************************************************
tBoolean
GPIOPullEnable(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC0;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL0) != GPIO_PEN_PULL0);
    case 1:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC1;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL1) != GPIO_PEN_PULL1);
    case 2:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC2;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL2) != GPIO_PEN_PULL2);
    case 3:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC3;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL3) != GPIO_PEN_PULL3);
    case 4:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC4;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL4) != GPIO_PEN_PULL4);
    case 5:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC5;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL5) != GPIO_PEN_PULL5);
    case 6:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC6;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL6) != GPIO_PEN_PULL6);
    case 7:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC7;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL7) != GPIO_PEN_PULL7);
    case 8:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC8;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL8) != GPIO_PEN_PULL8);
    case 9:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC9;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL9) != GPIO_PEN_PULL9);
    case 10:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC10;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL10) != GPIO_PEN_PULL10);
    case 11:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC11;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL11) != GPIO_PEN_PULL11);
    case 12:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC12;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL12) != GPIO_PEN_PULL12);
    case 13:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC13;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL13) != GPIO_PEN_PULL13);
    case 14:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC14;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL14) != GPIO_PEN_PULL14);
    case 15:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC15;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL15) != GPIO_PEN_PULL15);
    case 16:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC16;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL16) != GPIO_PEN_PULL16);
    case 17:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC17;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL17) != GPIO_PEN_PULL17);
    case 18:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC18;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL18) != GPIO_PEN_PULL18);
    case 19:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC19;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL19) != GPIO_PEN_PULL19);
    case 20:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC20;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL20) != GPIO_PEN_PULL20);
    case 21:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC21;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL21) != GPIO_PEN_PULL21);
    case 22:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC22;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL22) != GPIO_PEN_PULL22);
    case 23:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC23;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL23) != GPIO_PEN_PULL23);
    case 24:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC24;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL24) != GPIO_PEN_PULL24);
    case 25:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC25;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL25) != GPIO_PEN_PULL25);
    case 26:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC26;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL26) != GPIO_PEN_PULL26);
    case 27:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC27;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL27) != GPIO_PEN_PULL27);
    case 28:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC28;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL28) != GPIO_PEN_PULL28);
    case 29:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC29;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL29) != GPIO_PEN_PULL29);
    case 30:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC30;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL30) != GPIO_PEN_PULL30);
    case 31:
        HWREG(ulBase + GPIO_O_PENC) |= GPIO_PENC_PULLC31;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL31) != GPIO_PEN_PULL31);
    default:
        return(false);
    }
}

//*****************************************************************************
//
//! Disable GPIO internal pull resistor(pull down or pull up is pin dependence).
//!
//! \param ulBase is the base address of the GPIO port.
//! \param ulPinNumber is the number of the GPIO pin.
//!
//! This function disable gpio internal pull register.
//!
//! \return Returns true if disable pull resistero operation success, others false
//
//*****************************************************************************
tBoolean
GPIOPullDisable(unsigned long ulBase, unsigned long ulPinNumber)
{
    //
    // Check the arguments.
    //
    ASSERT(GPIOBaseValid(ulBase));
    ASSERT(GPIOPinNumberValid(ulPinNumber));

    switch (ulPinNumber) {
    case 0:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS0;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL0) == GPIO_PEN_PULL0);
    case 1:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS1;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL1) == GPIO_PEN_PULL1);
    case 2:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS2;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL2) == GPIO_PEN_PULL2);
    case 3:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS3;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL3) == GPIO_PEN_PULL3);
    case 4:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS4;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL4) == GPIO_PEN_PULL4);
    case 5:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS5;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL5) == GPIO_PEN_PULL5);
    case 6:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS6;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL6) == GPIO_PEN_PULL6);
    case 7:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS7;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL7) == GPIO_PEN_PULL7);
    case 8:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS8;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL8) == GPIO_PEN_PULL8);
    case 9:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS9;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL9) == GPIO_PEN_PULL9);
    case 10:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS10;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL10) == GPIO_PEN_PULL10);
    case 11:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS11;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL11) == GPIO_PEN_PULL11);
    case 12:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS12;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL12) == GPIO_PEN_PULL12);
    case 13:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS13;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL13) == GPIO_PEN_PULL13);
    case 14:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS14;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL14) == GPIO_PEN_PULL14);
    case 15:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS15;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL15) == GPIO_PEN_PULL15);
    case 16:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS16;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL16) == GPIO_PEN_PULL16);
    case 17:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS17;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL17) == GPIO_PEN_PULL17);
    case 18:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS18;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL18) == GPIO_PEN_PULL18);
    case 19:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS19;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL19) == GPIO_PEN_PULL19);
    case 20:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS20;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL20) == GPIO_PEN_PULL20);
    case 21:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS21;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL21) == GPIO_PEN_PULL21);
    case 22:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS22;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL22) == GPIO_PEN_PULL22);
    case 23:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS23;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL23) == GPIO_PEN_PULL23);
    case 24:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS24;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL24) == GPIO_PEN_PULL24);
    case 25:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS25;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL25) == GPIO_PEN_PULL25);
    case 26:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS26;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL26) == GPIO_PEN_PULL26);
    case 27:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS27;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL27) == GPIO_PEN_PULL27);
    case 28:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS28;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL28) == GPIO_PEN_PULL28);
    case 29:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS29;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL29) == GPIO_PEN_PULL29);
    case 30:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS30;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL30) == GPIO_PEN_PULL30);
    case 31:
        HWREG(ulBase + GPIO_O_PENS) |= GPIO_PENS_PULLS31;
        return((HWREG(ulBase + GPIO_O_PEN) & GPIO_PEN_PULL31) == GPIO_PEN_PULL31);
    default:
        return(false);
    }
}
