/*
 * Interrupt Controller
 */

#include "../inc/hw_intc.h"
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "intc.h"
#include "debug.h"

//*****************************************************************************
//
//! Enables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.
//!
//! \return None.
//
//*****************************************************************************
void
IntEnable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUSM);

    switch (ulInterrupt) {
	    case IRQ_NO_DMIC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DMIC;
		    break;
	    case IRQ_NO_AIC0 :
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_AIC0;
		    break;
	    case IRQ_NO_BCH:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_BCH;
		    break;
	    case IRQ_NO_DSI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DSI;
		    break;
	    case IRQ_NO_CSI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_CSI;
		    break;
	    case IRQ_NO_OHCI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_OHCI;
		    break;
	    case IRQ_NO_IPU:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_IPU;
		    break;
	    case IRQ_NO_SSI1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SSI1;
		    break;
	    case IRQ_NO_SSI0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SSI0;
		    break;
	    case IRQ_NO_Reserved0:
		    break;
	    case IRQ_NO_PDMA:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_PDMA;
		    break;
	    case IRQ_NO_Reserved1:
		    break;
	    case IRQ_NO_GPIO5:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO5;
		    break;
	    case IRQ_NO_GPIO4:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO4;
		    break;
	    case IRQ_NO_GPIO3:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO3;
		    break;
	    case IRQ_NO_GPIO2:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO2;
		    break;
	    case IRQ_NO_GPIO1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO1;
		    break;
	    case IRQ_NO_GPIO0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_GPIO0;
		    break;
	    case IRQ_NO_SADC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_SADC;
		    break;
	    case IRQ_NO_EPDC:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_EPDC;
		    break;
	    case IRQ_NO_EHCI:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_EHCI;
		    break;
	    case IRQ_NO_OTG:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_OTG;
		    break;
	    case IRQ_NO_HASH:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_HASH;
		    break;
	    case IRQ_NO_AES:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_AES;
		    break;
	    case IRQ_NO_Reserved2:
		    break;
	    case IRQ_NO_TCU2:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU2;
		    break;
	    case IRQ_NO_TCU1:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU1;
		    break;
	    case IRQ_NO_TCU0:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_TCU0;
		    break;
	    case IRQ_NO_Reserved3:
		    break;
	    case IRQ_NO_ISP:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_ISP;
		    break;
	    case IRQ_NO_DELAY_LINE:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_DELAY_LINE;
		    break;
	    case IRQ_NO_LCD:
		    HWREG(INTC_BASE + INTC_O_MCR0) |= INTC_MCR0_LCD;
		    break;
	    case IRQ_NO_RTC:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_RTC;
		    break;
	    case IRQ_NO_Reserved4:
		    break;
	    case IRQ_NO_UART4:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART4;
		    break;
	    case IRQ_NO_MSC2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC2;
		    break;
	    case IRQ_NO_MSC1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC1;
		    break;
	    case IRQ_NO_MSC0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_MSC0;
		    break;
	    case IRQ_NO_Reserved5:
		    break;
	    case IRQ_NO_NFI:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_NFI;
		    break;
	    case IRQ_NO_PCM0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_PCM0;
		    break;
	    case IRQ_NO_Reserved6:
	    case IRQ_NO_Reserved7:
	    case IRQ_NO_Reserved8:
		    break;
	    case IRQ_NO_HARB2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB2;
		    break;
	    case IRQ_NO_HARB1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB1;
		    break;
	    case IRQ_NO_HARB0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_HARB0;
		    break;
	    case IRQ_NO_CPM:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_CPM;
		    break;
	    case IRQ_NO_UART3:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART3;
		    break;
	    case IRQ_NO_UART2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART2;
		    break;
	    case IRQ_NO_UART1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART1;
		    break;
	    case IRQ_NO_UART0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_UART0;
		    break;
	    case IRQ_NO_DDR:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_DDR;
		    break;
	    case IRQ_NO_Reserved9:
		    break;
	    case IRQ_NO_EFUSE:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_EFUSE;
		    break;
	    case IRQ_NO_ETHC:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_ETHC;
		    break;
	    case IRQ_NO_Reserved10:
		    break;
	    case IRQ_NO_I2C3:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C3;
		    break;
	    case IRQ_NO_I2C2:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C2;
		    break;
	    case IRQ_NO_I2C1:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C1;
		    break;
	    case IRQ_NO_I2C0:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_I2C0;
		    break;
	    case IRQ_NO_PDMAM:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_PDMAM;
		    break;
	    case IRQ_NO_VPU:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_VPU;
		    break;
	    case IRQ_NO_GPU:
		    HWREG(INTC_BASE + INTC_O_MCR1) |= INTC_MCR1_GPU;
		    break;
	    default:
		    break;
    }
}

//*****************************************************************************
//
//! Disables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.
//!
//! \return None.
//
//*****************************************************************************
void
IntDisable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < INTC_INTERRUPT_NUSM);

    switch (ulInterrupt) {
	    case IRQ_NO_DMIC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DMIC;
		    break;
	    case IRQ_NO_AIC0 :
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_AIC0;
		    break;
	    case IRQ_NO_BCH:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_BCH;
		    break;
	    case IRQ_NO_DSI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DSI;
		    break;
	    case IRQ_NO_CSI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_CSI;
		    break;
	    case IRQ_NO_OHCI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_OHCI;
		    break;
	    case IRQ_NO_IPU:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_IPU;
		    break;
	    case IRQ_NO_SSI1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SSI1;
		    break;
	    case IRQ_NO_SSI0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SSI0;
		    break;
	    case IRQ_NO_Reserved0:
		    break;
	    case IRQ_NO_PDMA:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_PDMA;
		    break;
	    case IRQ_NO_Reserved1:
		    break;
	    case IRQ_NO_GPIO5:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO5;
		    break;
	    case IRQ_NO_GPIO4:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO4;
		    break;
	    case IRQ_NO_GPIO3:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO3;
		    break;
	    case IRQ_NO_GPIO2:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO2;
		    break;
	    case IRQ_NO_GPIO1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO1;
		    break;
	    case IRQ_NO_GPIO0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_GPIO0;
		    break;
	    case IRQ_NO_SADC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_SADC;
		    break;
	    case IRQ_NO_EPDC:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_EPDC;
		    break;
	    case IRQ_NO_EHCI:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_EHCI;
		    break;
	    case IRQ_NO_OTG:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_OTG;
		    break;
	    case IRQ_NO_HASH:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_HASH;
		    break;
	    case IRQ_NO_AES:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_AES;
		    break;
	    case IRQ_NO_Reserved2:
		    break;
	    case IRQ_NO_TCU2:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU2;
		    break;
	    case IRQ_NO_TCU1:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU1;
		    break;
	    case IRQ_NO_TCU0:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_TCU0;
		    break;
	    case IRQ_NO_Reserved3:
		    break;
	    case IRQ_NO_ISP:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_ISP;
		    break;
	    case IRQ_NO_DELAY_LINE:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_DELAY_LINE;
		    break;
	    case IRQ_NO_LCD:
		    HWREG(INTC_BASE + INTC_O_MSR0) |= INTC_MSR0_LCD;
		    break;
	    case IRQ_NO_RTC:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_RTC;
		    break;
	    case IRQ_NO_Reserved4:
		    break;
	    case IRQ_NO_UART4:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART4;
		    break;
	    case IRQ_NO_MSC2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC2;
		    break;
	    case IRQ_NO_MSC1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC1;
		    break;
	    case IRQ_NO_MSC0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_MSC0;
		    break;
	    case IRQ_NO_Reserved5:
		    break;
	    case IRQ_NO_NFI:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_NFI;
		    break;
	    case IRQ_NO_PCM0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_PCM0;
		    break;
	    case IRQ_NO_Reserved6:
	    case IRQ_NO_Reserved7:
	    case IRQ_NO_Reserved8:
		    break;
	    case IRQ_NO_HARB2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB2;
		    break;
	    case IRQ_NO_HARB1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB1;
		    break;
	    case IRQ_NO_HARB0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_HARB0;
		    break;
	    case IRQ_NO_CPM:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_CPM;
		    break;
	    case IRQ_NO_UART3:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART3;
		    break;
	    case IRQ_NO_UART2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART2;
		    break;
	    case IRQ_NO_UART1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART1;
		    break;
	    case IRQ_NO_UART0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_UART0;
		    break;
	    case IRQ_NO_DDR:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_DDR;
		    break;
	    case IRQ_NO_Reserved9:
		    break;
	    case IRQ_NO_EFUSE:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_EFUSE;
		    break;
	    case IRQ_NO_ETHC:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_ETHC;
		    break;
	    case IRQ_NO_Reserved10:
		    break;
	    case IRQ_NO_I2C3:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C3;
		    break;
	    case IRQ_NO_I2C2:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C2;
		    break;
	    case IRQ_NO_I2C1:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C1;
		    break;
	    case IRQ_NO_I2C0:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_I2C0;
		    break;
	    case IRQ_NO_PDMAM:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_PDMAM;
		    break;
	    case IRQ_NO_VPU:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_VPU;
		    break;
	    case IRQ_NO_GPU:
		    HWREG(INTC_BASE + INTC_O_MSR1) |= INTC_MSR1_GPU;
		    break;
	    default:
		    break;
    }
}

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
INTCRegisterDump(int (*print)(const char *format, ...))
{
	REG_PRINT(INTC_O_SR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MSR0, INTC_BASE, print);
	REG_PRINT(INTC_O_MCR0, INTC_BASE, print);
	REG_PRINT(INTC_O_PR0, INTC_BASE, print);
	REG_PRINT(INTC_O_SR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MSR1, INTC_BASE, print);
	REG_PRINT(INTC_O_MCR1, INTC_BASE, print);
	REG_PRINT(INTC_O_PR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DSR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DMR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DPR0, INTC_BASE, print);
	REG_PRINT(INTC_O_DSR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DMR1, INTC_BASE, print);
	REG_PRINT(INTC_O_DPR1, INTC_BASE, print);
}

//*****************************************************************************
//
//! \Get INTC_O_SR0
//!
//! \param none.
//! \return Return the value of register INTC_O_SR0
//!
//*****************************************************************************
unsigned long
INTCSr0Get()
{
	return(HWREG(INTC_BASE + INTC_O_SR0));
}

//*****************************************************************************
//
//! \Get INTC_O_SR1
//!
//! \param none.
//! \return Return the value of INTC_O_SR1.
//!
//*****************************************************************************
unsigned long
INTCSr1Get()
{
	return(HWREG(INTC_BASE + INTC_O_SR1));
}
