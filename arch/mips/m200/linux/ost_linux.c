/*
 * Operating System Timer(OST)
 */
#include <linux/interrupt.h>
#include <linux/io.h> /* writel/readl */
#include <linux/printk.h> /* printk */
#include <linux/irq.h> /* IRQ_HANDLED */
#include <linux/clockchips.h> /* clockevents_config_and_register */
#include <linux/init.h>
#include "../driverlib/ost.h"
#include "../driverlib/intc.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_ost.h"
#include "../inc/hw_tcu.h"

static void ost_init(void)
{
	unsigned int ost_irq;
	
    /* 1. Initial the configuration */
    /* a. Set the shutdown mode(abrupt) */
    OSTCounterModeSet(OST_BASE, OST_COUNTER_GOON);
    OSTShutdown(OST_BASE, OST_SHUTDOWN_ABRUPT);
	
	/* b. Set OSTCNT count clock frequency prescale */
    OSTClockInputPrescaleSet(OST_BASE, OST_CLOCKPRESCALE_256);

    /* c. Set counter register(64-bit) */
    OSTCounterSet(OST_BASE, 0x00000000, 0x00000000);

    /* d. Set data register(32-bit) */
    OSTDataSet(OST_BASE, 0x00000000);

	/* 2. Enable clock */
    OSTClockInputSet(OST_BASE, OST_CLOCKINPUT_EXTAL);
    OSTClockSupply(OST_BASE);

	/* 3. Enable OST counter(start increase) */
	OSTCounterEnable(OST_BASE);

    /* 4. We not use OST interrupt, so close it */
	/* a. OSTFLAG clear */
    OSTComparisonMatchFlagClear(OST_BASE);
    OSTRegisterDump(OST_BASE, printk);

	/* b. Mask OST interrupt	 */
    OSTInterruptMask(OST_BASE);

    /* c. Disable INTC OST interrupt switch */
    ost_irq = OSTIntNumberGet(OST_BASE);
    if (ost_irq == -1) {
        printk("Error OSTIntNumberGet failed\n");
    } else {
	    INTCInterruptDisable(ost_irq);
    }

	printk("%s line:%d\n", __func__, __LINE__);

    OSTRegisterDump(OST_BASE, printk);
}


static cycle_t ost_cycle_read(struct clocksource *cs)
{
    cycle_t res;
    unsigned int *pHigh, *pLow;

    (void)cs;
    /* Arch must be little-endian */
    pLow = (unsigned int*)&res;
    pHigh = pLow + 1;

    OSTCounterGet(OST_BASE, (unsigned long *)pHigh, (unsigned long *)pLow);

    return res;
}

static int ost_clocksource_enable(struct clocksource *cs)
{
    printk("%s line:%d\n", __func__, __LINE__);
    (void)cs;
    OSTCounterEnable(OST_BASE);
    return 0;
}

static void ost_clocksource_disable(struct clocksource *cs)
{
    printk("%s line:%d\n", __func__, __LINE__);
    (void)cs;
    OSTCounterDisable(OST_BASE);
}

/* Stop clock supply */
static void ost_clocksource_suspend(struct clocksource *cs)
{
    printk("%s line:%d\n", __func__, __LINE__);
    (void)cs;
    OSTClockNotSupply(OST_BASE);
}

/* Resume clock supply */
static void ost_clocksource_resume(struct clocksource *cs)
{
    printk("%s line:%d\n", __func__, __LINE__);
    (void)cs;
    OSTClockSupply(OST_BASE);
}

static struct clocksource cs = {
    .read = ost_cycle_read,
    .name = "'ost clocksource'",
    .rating = 400,
    .mask = 0x7FFFFFFFFFFFFFFFULL,
    .mult = 1,
    .shift = 8,
    .flags = CLOCK_SOURCE_WATCHDOG | CLOCK_SOURCE_IS_CONTINUOUS,
    .enable = ost_clocksource_enable,
    .disable = ost_clocksource_disable,
    .suspend = ost_clocksource_suspend,
    .resume = ost_clocksource_resume,
};

/*
 * Override sched_clock() in /kernel/sched/clock.c
 * This is must be done.
 * using cs.mult and cs.shift make the calculation more precise
 */
unsigned long long sched_clock(void)
{
    unsigned long long result = (ost_cycle_read((struct clocksource *)0) * cs.mult) >> cs.shift;
    return result;
}

void __init ost_clocksource_init(void)
{
    /* Override cs.mult */
    cs.mult = clocksource_hz2mult(24000000/256, cs.shift),

	ost_init();
    clocksource_register(&cs);
}
