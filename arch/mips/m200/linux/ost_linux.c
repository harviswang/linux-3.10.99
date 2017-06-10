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
#include "../inc/hw_tcu.h"
#include "../inc/hw_ost.h"

static void ost_init(void)
{
	unsigned int i = 1;
	
    /* 1. Initial the configuration */
    /* a. Set the shutdown mode(graceful) */
    OSTSetCounterMode(OST_BASE, OST_COUNTER_BECLEARED);
    OSTShutdown(OST_BASE, OST_SHUTDOWN_GRACEFUL);
	
	/* b. Set OSTCNT count clock frequency prescale */
    OSTSetClockInputPrescale(OST_BASE, OST_CLOCKPRESCALE_1);

    /* c. Set counter register(64-bit) */
    OSTSetCounter(OST_BASE, 0x00000000, 0x00000000);

    /* d. Set data register(32-bit) */
    OSTSetData(OST_BASE, 24000000/100);

	IntEnable(IRQ_NO_TCU0);

	/* 2. Enable clock */
    OSTSetClockInput(OST_BASE, OST_CLOCKINPUT_EXTAL); 

	/*
	 * OSTFLAG 
	 */
	writel(1 << 15, (void*)(TCU + TCU_TFSR));
	printk("%s line:%d\n", __func__, __LINE__);

	/* 3. Enable OST counter(start increase) */
	writel(1 << 15, (void*)(TCU + TCU_TESR));

	/*
	 * OSTMASK
	 */
	writel(1 << 15, (void*)(TCU + TCU_TMCR));
	printk("%s line:%d\n", __func__, __LINE__);

	printk("%s line:%d\n", __func__, __LINE__);
	while (i < 1000) {
		if (readl((void*)(OST_BASE + OST_O_CNTL)) == readl((void*)(OST_BASE + OST_O_DR))) {
			printk("OST_O_CNTL == OST_O_DR i:0x%x\n", i);
		}
		i++;
	}
	printk("OST_O_CNTH = 0x%x\n", readl((void*)(OST_BASE + OST_O_CNTH)));
	printk("OST_O_CNTL = 0x%x\n", readl((void*)(OST_BASE + OST_O_CNTL)));
	printk("OST_O_DR = 0x%x\n", readl((void*)(OST_BASE + OST_O_DR)));
	//printk("INTC_ICSR0 = 0x%x\n", readl((void*)(INTC + INTC_ICSR0)));
	//printk("INTC_ICPR0 = 0x%x\n", readl((void*)(INTC + INTC_ICPR0)));
	//printk("INTC_ICMR0 = 0x%x\n", readl((void*)(INTC + INTC_ICMR0)));
	printk("TCU_TSR = 0x%x\n", readl((void*)(TCU + TCU_TSR)));
    OSTRegisterDump(OST_BASE, printk);
}

static void ost_start(void)
{
	writel(1 << 15, (void*)(TCU + TCU_TESR));
}

/*
 * OST stop counting up
 */
static void ost_stop(void)
{
	writel(1 << 15, (void*)(TCU + TCU_TECR));
}

static void ost_count_reset(void)
{
	ost_stop();
	writel(0x00, (void*)(OST_BASE + OST_O_CNTH));
	writel(0x00, (void*)(OST_BASE + OST_O_CNTL));
	ost_start();
}

static irqreturn_t ost_interrupt_handle(int irq, void *dev_id)
{
	struct clock_event_device *ced = dev_id;
	writel(1 << 15, (void*)(TCU + TCU_TFCR));
	//printk("%s line:%d\n", __func__, __LINE__);
	
	ced->event_handler(ced);

	return IRQ_HANDLED;
}

static void
ost_set_mode(enum clock_event_mode mode, struct clock_event_device *ced)
{
	switch(mode) {
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		break;
	case CLOCK_EVT_MODE_PERIODIC:
		ost_count_reset();
		break;
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_RESUME:
		break;
		
	}
	(void)ced;
}

static int
ost_set_next_event(unsigned long delta, struct clock_event_device *ced)
{
	(void)delta;
	(void)ced;
	
	printk("%s line:%d\n", __func__, __LINE__);
	return 0;
}

void __init ost_time_init(void)
{
	static struct irqaction action;
	static struct clock_event_device ced;
	
	ost_init();

	action.name = "OST IRQ";
	action.handler = ost_interrupt_handle;
	action.flags = IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL;
	action.dev_id = (void *)&ced;
	setup_irq(IRQ_NO_TCU0, &action);
	
	ced.name = "OST EVENT";
	ced.features = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC;
	ced.irq = IRQ_NO_TCU0;
	ced.rating = 400,
	ced.set_mode = ost_set_mode;
	ced.set_next_event = ost_set_next_event;
	clockevents_config_and_register(&ced, 1, 4, 65536);
}
