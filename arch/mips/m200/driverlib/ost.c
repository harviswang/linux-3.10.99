/*
 * Operating System Timer(OST)
 */

#include "../inc/hw_tcu.h"
#include "../inc/hw_ost.h"
#include "../inc/hw_intc.h"
#include <linux/interrupt.h>
#include <linux/io.h> /* writel/readl */
#include <linux/printk.h> /* printk */
#include <linux/irq.h> /* IRQ_HANDLED */
#include <linux/clockchips.h> /* clockevents_config_and_register */
#include "intc.h"

void ost_counter_mode_set(void)
{
	
}

void ost_counter_shutdown(void)
{
	
}

static void
ost_init(void)
{
	unsigned int i = 1;
	
	writew(0x0000, (void*)(OST + OST_CSR));
	
	/*
	 *  
	 */
	printk("OST_CNTH = 0x%x\n", readl((void*)(OST + OST_CNTH)));
	printk("OST_CNTL = 0x%x\n", readl((void*)(OST + OST_CNTL)));
	writel(0x00, (void*)(OST + OST_CNTH));
	writel(0x00, (void*)(OST + OST_CNTL));
	printk("%s line:%d\n", __func__, __LINE__);
	/*
	 * OSTDR = 24M(0x1800000)
	 * 
	 */
	writel(240000/100, (void*)(OST + OST_DR));

	/*
	 * 
	 */
	printk("%s line:%d\n", __func__, __LINE__);
	printk("%s line:%d\n", __func__, __LINE__);
	IntEnable(IRQ_NO_TCU0);
	printk("%s line:%d\n", __func__, __LINE__);

	/*
	 * CNT_CMD = 0
	 * SD = 0
	 * PRESCALE = 0
	 * EXT_EN = 1
	 * RTC_EN = 0
	 * PCK = 0
	 */
	writew(1 << 2, (void*)(OST + OST_CSR));
	printk("%s line:%d\n", __func__, __LINE__);

	/*
	 * OSTFLAG 
	 */
	writel(1 << 15, (void*)(TCU + TCU_TFSR));
	printk("%s line:%d\n", __func__, __LINE__);
	/*
	 * OST Enable
	 */
	writel(1 << 15, (void*)(TCU + TCU_TESR));

	/*
	 * OSTMASK
	 */
	writel(1 << 15, (void*)(TCU + TCU_TMCR));
	printk("%s line:%d\n", __func__, __LINE__);

	printk("%s line:%d\n", __func__, __LINE__);
	while (i < 1000) {
		if (readl((void*)(OST + OST_CNTL)) == readl((void*)(OST + OST_DR))) {
			printk("OST_CNTL == OST_DR i:0x%x\n", i);
		}
		i++;
	}
	printk("OST_CNTH = 0x%x\n", readl((void*)(OST + OST_CNTH)));
	printk("OST_CNTL = 0x%x\n", readl((void*)(OST + OST_CNTL)));
	printk("OST_DR = 0x%x\n", readl((void*)(OST + OST_DR)));
	//printk("INTC_ICSR0 = 0x%x\n", readl((void*)(INTC + INTC_ICSR0)));
	//printk("INTC_ICPR0 = 0x%x\n", readl((void*)(INTC + INTC_ICPR0)));
	//printk("INTC_ICMR0 = 0x%x\n", readl((void*)(INTC + INTC_ICMR0)));
	printk("TCU_TSR = 0x%x\n", readl((void*)(TCU + TCU_TSR)));
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
	writel(0x00, (void*)(OST + OST_CNTH));
	writel(0x00, (void*)(OST + OST_CNTL));
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

void __init ost_time_init()
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
