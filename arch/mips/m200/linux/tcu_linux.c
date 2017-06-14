/*
 * Timer Counter Unit(TCU)
 */
#include <linux/interrupt.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <linux/init.h>
#include <linux/io.h> /* readl/writel */
#include "../driverlib/tcu.h"
#include "../driverlib/intc.h"
#include "../inc/hw_tcu.h"
#include "../inc/hw_memmap.h"

static void tcu_timer5_init(void)
{
    unsigned int timer5_irq = 0;

    /* 1. Initial the configuration */
    /* a. Writing TCSR.INITL to initialize PWM output level. */
    HWREGH(TCU_BASE + TCU_O_CSR5) &= ~TCU_CSR5_INITL;

    /* b. Set shutdown mode */
    TCUPWMShutdown(TCU_BASE, TCU_TIMER5, TCU_SHUTDOWN_GRACEFUL);

    /* c. Set prescale */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKPRESCALE_64);

    /* Setting TCNT, TDHR and TDFR */
    HWREGH(TCU_BASE + TCU_O_CNT5) = 0x0000;
    HWREGH(TCU_BASE + TCU_O_DHR5) = 0x8000;
    HWREGH(TCU_BASE + TCU_O_DFR5) = 0xFFFF;

    /* 2. Enable the clock */
    /* a. Writing TCSR.PWM_EN to set whether enable PWM or disable PWM */
    HWREGH(TCU_BASE + TCU_O_CSR5) &= ~TCU_CSR5_PWMEN;

    /* b. Set input clock */
    TCUClockInputSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER5);

    /* 3. Enable the counter. */
    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
    printk("file:%s line:%d\n", __FILE__, __LINE__);

    HWREG(TCU_BASE + TCU_O_MCR) |= TCU_MCR_HMCL5;
    HWREG(TCU_BASE + TCU_O_MCR) |= TCU_MCR_FIFOMCL5;
    timer5_irq = TCUIntNumberGet(TCU_TIMER5);
    if (timer5_irq == -1) {
        printk("TCUIntNumberGet() failed\n");
    } else {
        INTCInterruptEnable(timer5_irq);
    }

    TCURegisterDump(TCU_BASE, printk);
}

static void tcu_timer5_start(void)
{
    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
//    TCUClockSupply(TCU_BASE, TCU_TIMER5);
}

static void tcu_timer5_stop(void)
{
    TCUCounterDisable(TCU_BASE, TCU_TIMER5);
//    TCUClockNotSupply(TCU_BASE, TCU_TIMER5);
}

static void tcu_timer5_count_reset(void)
{
    tcu_timer5_stop();
    HWREGH(TCU_BASE + TCU_O_CNT5) = 0x0000;
    tcu_timer5_start();
}

static irqreturn_t
tcu_timer5_interrupt_handle(int irq, void *dev_id)
{
    struct clock_event_device *ced = dev_id;

    HWREG(TCU_BASE + TCU_O_FCR) |= TCU_FCR_HFCL5;
    HWREG(TCU_BASE + TCU_O_FCR) |= TCU_FCR_FFCL5;
    //printk("%s line:%d\n", __func__, __LINE__);

    ced->event_handler(ced);

    return IRQ_HANDLED;
}

static void
tcu_timer5_set_mode(enum clock_event_mode mode, struct clock_event_device *ced)
{
    switch(mode) {
    case CLOCK_EVT_MODE_UNUSED:
    case CLOCK_EVT_MODE_SHUTDOWN:
        break;
    case CLOCK_EVT_MODE_PERIODIC:
        tcu_timer5_count_reset();
        break;
    case CLOCK_EVT_MODE_ONESHOT:
    case CLOCK_EVT_MODE_RESUME:
        break;
    default:
        printk("%s line:%d\n", __func__, __LINE__);
        break;
    }
    (void)ced;
}

static int
tcu_timer5_set_next_event(unsigned long delta, struct clock_event_device *ced)
{
    (void)delta;
    (void)ced;

    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

void __init tcu_timer5_clockevent_init(void)
{
    static struct irqaction action;
    static struct clock_event_device ced;

    tcu_timer5_init();

    action.name = "'TCU Timer5 IRQ'";
    action.handler = tcu_timer5_interrupt_handle;
    action.flags = IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL;
    action.dev_id = (void *)&ced;
    setup_irq(IRQ_NO_TCU1, &action);

    ced.name = "'TCU Timer5 Clock Event'";
    ced.features = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC;
    ced.irq = IRQ_NO_TCU1;
    ced.rating = 400,
    ced.set_mode = tcu_timer5_set_mode;
    ced.set_next_event = tcu_timer5_set_next_event;
    clockevents_config_and_register(&ced, 1, 4, 65536);
}
