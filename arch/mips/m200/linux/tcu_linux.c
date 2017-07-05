/*
 * Timer Counter Unit(TCU)
 */
#include <linux/interrupt.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <linux/init.h>
#include <linux/irqdomain.h>
#include <linux/io.h> /* readl/writel */
#include "../driverlib/tcu.h"
#include "../driverlib/intc.h"
#include "../inc/hw_tcu.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_types.h"

static void tcu_timer5_init(void)
{
    unsigned int timer5_hw_irq = 0;
    printk("%s line:%d\n", __func__, __LINE__);

    /* 1. Initial the configuration */
    /* a. Writing TCSR.INITL to initialize PWM output level. */
    TCUPWMInitialOutputLevelSet(TCU_BASE, TCU_TIMER5, TCU_PWMINITOUTPUT_LOW);

    /* b. Set shutdown mode */
    TCUPWMShutdown(TCU_BASE, TCU_TIMER5, TCU_SHUTDOWN_GRACEFUL);

    /* c. Set prescale */
    TCUClockInputPrescaleSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKPRESCALE_4);

    /* Setting TCNT, TDHR and TDFR */
    TCUCounterSet(TCU_BASE, TCU_TIMER5, 0x0000);
    TCUDataHalfSet(TCU_BASE, TCU_TIMER5, 0xFFFF); // Half > Full, we not use half comparison here
    TCUDataFullSet(TCU_BASE, TCU_TIMER5, 60000 - 1); // 24000000/4/HZ = 60000

    /* 2. Enable the clock */
    /* a. Writing TCSR.PWM_EN to set whether enable PWM or disable PWM */
    TCUPWMDisable(TCU_BASE, TCU_TIMER5);

    /* b. Set input clock */
    TCUClockInputSet(TCU_BASE, TCU_TIMER5, TCU_CLOCKINPUT_EXTAL);
    TCUClockSupply(TCU_BASE, TCU_TIMER5);

    /* c. Mask non-full interrupt of timer 5, unmask full interrupt of timer 5 */
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_HALF);
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FIFOEMPTY);
    TCUInterruptMask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FIFO);
    TCUInterruptUnmask(TCU_BASE, TCU_TIMER5, TCU_MASKTYPE_FULL);
    timer5_hw_irq = TCUIntNumberGet(TCU_TIMER5);
    if (timer5_hw_irq == -1) {
        printk("TCUIntNumberGet() failed\n");
    } else {
        INTCInterruptEnable(timer5_hw_irq);
    }

    /* 3. Enable the counter. */
    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
    printk("file:%s line:%d\n", __FILE__, __LINE__);

}

static void tcu_timer5_start(void)
{
//    TCUCounterEnable(TCU_BASE, TCU_TIMER5);
    TCUClockSupply(TCU_BASE, TCU_TIMER5);
}

static void tcu_timer5_stop(void)
{
//    TCUCounterDisable(TCU_BASE, TCU_TIMER5);
    TCUClockNotSupply(TCU_BASE, TCU_TIMER5);
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

    if (HWREG(TCU_BASE + TCU_O_FR) & TCU_FR_FFLAG5) {
        HWREG(TCU_BASE + TCU_O_FCR) |= TCU_FCR_FFCL5;
// TCNT will become zero, after match full data register
//        TCUCounterSet(TCU_BASE, TCU_TIMER5, 0x0000);
    }

    //printk("%s line:%d\n", __func__, __LINE__);

    ced->event_handler(ced);

    return IRQ_HANDLED;
}

/*
 * TODO
 */
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

/*
 * TODO
 */
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
    unsigned int timer5_virtual_irq;

    tcu_timer5_init();

    /* hw irq -> virtual irq */
    timer5_virtual_irq = irq_find_mapping(NULL, IRQ_NO_TCU1);
    //printk("%s %d timert_virtual_irq:%d\n", __func__, __LINE__, timer5_virtual_irq);

    action.name = "'TCU Timer5 IRQ'";
    action.handler = tcu_timer5_interrupt_handle;
    action.flags = IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL;
    action.dev_id = (void *)&ced;
    setup_irq(timer5_virtual_irq, &action);

    ced.name = "'TCU Timer5 Clock Event'";
    ced.features = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC;
    ced.irq = timer5_virtual_irq;
    ced.rating = 400,
    ced.set_mode = tcu_timer5_set_mode;
    ced.set_next_event = tcu_timer5_set_next_event;
    clockevents_config_and_register(&ced, 1, 4, 65536);
}
