/*
 * invoked in arch/mips/kernel/irq.c:init_IRQ()
 */

#include <linux/init.h>
#include <linux/module.h>
#include <asm/irq_cpu.h>
#include <asm/mipsregs.h> /* clear_c0_cause */
#include <asm/setup.h>
#include <asm/irq.h> /* do_IRQ */
#include <asm/bitops.h> /* ffs */
#include "../driverlib/intc.h" /* IRQ_NO_TCU0 */
#include "../linux/intc_linux.h" /* intc_irq_init */

/*
 * interrupt 0: soft interrupt 0
 */
static void software_interrupt0_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP0);
}

/*
 * interrupt 1: soft interrupt 1
 */
static void software_interrupt1_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP1);
}

/*
 * interrupt 2: hardware interrupt 0
 */
static void hardware_interrupt0_handler(void)
{
	unsigned sr0 = (unsigned)INTCSr0Get();
	unsigned sr1 = (unsigned)INTCSr1Get();
	unsigned irq0, irq1;
	int i;

	for (i = 31; i >= 0; i--) {
		if (sr0 != 0) {
			irq0 = ffs(sr0);
			if (irq0 != 0) {
				irq0 -= 1;
		       		do_IRQ(irq0);
				sr0 &= ~(1 << irq0);
			}
		}

		if (sr1 != 0) {
			irq1 = ffs(sr1);
			if (irq1 != 0) {
				irq1 -= 1;
				do_IRQ(irq1 + 32);
				sr1 &= ~(1 << irq1);
			}
		}

		/* if none zero bit then break */
		if ((sr0 | sr1) == 0x00000000) {
			break;
		}
	}
	//do_IRQ(IRQ_NO_TCU0);
//	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP2);
}

/*
 * interrupt 3: hardware interrupt 1
 */
static void hardware_interrupt1_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP3);
}

/*
 * interrupt 4: hardware interrupt 2
 */
static void hardware_interrupt2_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP4);
}

/*
 * interrupt 5: hardware interrupt 3
 */
static void hardware_interrupt3_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP5);
}

/*
 * interrupt 6: hardware interrupt 4
 */
static void hardware_interrupt4_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP6);
}

/*
 * interrupt 7: hardware interrupt 5
 */
static void hardware_interrupt5_handler(void)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	clear_c0_cause(STATUSF_IP7);
}

void __init arch_init_irq(void)
{
	/*
	 * Initialize Software/Hardware interrupt handlers
	 */
	if (cpu_has_vint) {
		set_vi_handler(0, software_interrupt0_handler);
		set_vi_handler(1, software_interrupt1_handler);
		set_vi_handler(2, hardware_interrupt0_handler);
		set_vi_handler(3, hardware_interrupt1_handler);
		set_vi_handler(4, hardware_interrupt2_handler);
		set_vi_handler(5, hardware_interrupt3_handler);
		set_vi_handler(6, hardware_interrupt4_handler);
		set_vi_handler(7, hardware_interrupt5_handler);
	}

	/*
	 * INTC 64 interrupt initialize
	 */
	intc_irq_init();
}
