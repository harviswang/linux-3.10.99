/*
 * Interrupt Controller
 */

#include <linux/printk.h> /* printk */
#include <linux/irq.h> /* irq_set_chip_and_handler */
#include "../driverlib/intc.h"
#include "intc_linux.h"

static void intc_irq_mask(struct irq_data *data)
{
	IntDisable(data->irq);
}

static void intc_irq_unmask(struct irq_data *data)
{
	IntEnable(data->irq);
}

static struct irq_chip intc_chip = {
	.name = "interrupt controller irq",
	.irq_mask = intc_irq_mask,
	.irq_unmask = intc_irq_unmask,
};
/*
static void intc_handle_level_irq(unsigned int irq, struct irq_desc *desc)
{
	printk("file: %s function: %s line:%d\n", __FILE__, __func__, __LINE__);
	printk("desc->name: %s\n", desc->name);
	printk("irq:%d\n", irq);
	printk("desc->irq_data.irq:%d\n", desc->irq_data.irq);
	printk("desc->irq_data.hwirq:%ld\n", desc->irq_data.hwirq);
}
*/

void __init intc_irq_init()
{
	int irq;
	struct irq_chip *chip = &intc_chip;
	irq_flow_handler_t handle = handle_level_irq;

	for (irq = IRQ_NO_DMIC; irq <= IRQ_NO_GPU; irq++) {
		irq_set_chip_and_handler(irq, chip, handle);
	}
}
