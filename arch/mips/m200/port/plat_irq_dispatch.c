/*
 * arch/mips/kernel/genex.S:handle_int()
 */

#include <linux/kernel.h>
#include <asm/mipsregs.h> /* read_c0_cause/read_c0_status/ST0_IM */
#include <asm/irq.h> /* spurious_interrupt */

/*
 * plat_irq_dispatch is not used, use arch_init_irq()/set_vi_handler() instead
 * because M200 support VINT feature
 * this api is deprecated
 */
asmlinkage void plat_irq_dispatch(void)
{
	unsigned int pending;

	pending = read_c0_cause() & read_c0_status() & ST0_IM;
	if (pending & CAUSEF_IP2) {       /* Hardware interrupt 0 */
		printk("Hardware interrupt 0: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP3) { /* Hardware interrupt 1 */
		printk("Hardware interrupt 1: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP4) { /* Hardware interrupt 2 */
		printk("Hardware interrupt 2: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP5) { /* Hardware interrupt 3 */
		printk("Hardware interrupt 3: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP6) { /* Hardware interrupt 4 */
		printk("Hardware interrupt 4: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP7) { /* Hardware interrupt 5 */
		printk("Hardware interrupt 5: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP0) { /* Software interrupt 0 */
		printk("Software interrupt 0: %s line:%d\n", __func__, __LINE__);
	} else if (pending & CAUSEF_IP1) { /* Software interrupt 1 */
		printk("Software interrupt 1: %s line:%d\n", __func__, __LINE__);
	} else {
		printk("Unknow interrupt: %s line:%d\n", __func__, __LINE__);
		spurious_interrupt();
	}
	clear_c0_cause(pending);
}


