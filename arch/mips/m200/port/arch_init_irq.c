/*
 * invoked in arch/mips/kernel/irq.c:init_IRQ()
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioport.h> /* struct resource, request_mem_region */
#include <linux/irqdomain.h> /* struct irq_domain */
#include <linux/of_device.h>
#include <linux/of_irq.h> /* irq_of_parse_and_map */
#include <linux/of_address.h> /* of_address_to_resource/of_irq_init */
#include <linux/io.h> /* ioremap_nocache */
#include <asm/irq_cpu.h> /* mips_cpu_intc_init */
#include <asm/mipsregs.h> /* clear_c0_cause */
#include <asm/setup.h>
#include <asm/irq.h> /* do_IRQ */
#include <asm/bitops.h> /* ffs */
#include "../driverlib/intc.h" /* IRQ_NO_TCU0 */
#include "../linux/intc_linux.c" /* intc_chip */
#include "../linux/gpio_linux.c" /* gpio_irq_chip */

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
    const int intc_virq_offset = 8; /* This works with 8 cpu interrupt, covert hw irq to virtual irq number
                                     * irq0 as hw irq [0, 31], irq1 as hw irq [32, 63]
                                     */

	for (i = 31; i >= 0; i--) {
		if (sr0 != 0) {
			irq0 = ffs(sr0);
			if (irq0 != 0) {
				irq0 -= 1;
                do_IRQ(irq0 + intc_virq_offset);
				sr0 &= ~(1 << irq0);
			}
		}

		if (sr1 != 0) {
			irq1 = ffs(sr1);
			if (irq1 != 0) {
				irq1 -= 1;
				do_IRQ(irq1 + 32 + intc_virq_offset);
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

static int intc_map(struct irq_domain *id, unsigned int irq, irq_hw_number_t hw)
{
    //printk("%s line:%d irq:%d hw:%d\n", __func__, __LINE__, irq, (int)hw);
    irq_set_chip_and_handler(irq, &intc_chip, handle_level_irq);

    return 0;
}

static const struct irq_domain_ops intc_irq_domain_ops = {
    .xlate = irq_domain_xlate_onecell,
    .map   = intc_map,
};

/*
 * No choice to execute TBD, now using VINT
 */
static void m200_intc_irq_handler(unsigned int irq, struct irq_desc *desc)
{
    unsigned sr0 = (unsigned)INTCSr0Get();
	unsigned sr1 = (unsigned)INTCSr1Get();

    printk("%s line:%d\n", __func__, __LINE__);
    if (sr0 || sr1) {
        struct irq_domain *domain = irq_get_handler_data(irq);
        sr0 != 0 ? generic_handle_irq(irq_find_mapping(domain, __ffs(sr0))) : (void)sr0 ;
        sr1 != 0 ? generic_handle_irq(irq_find_mapping(domain, __ffs(sr1) + 32)) : (void)sr1 ;
    } else {
        spurious_interrupt();
    }
}

static int __init soc_intc_of_init(struct device_node *node,
                    struct device_node *parent)
{
    struct resource res;
    struct resource *pres;
    struct irq_domain *domain;
    int irq;
    int err;

    irq = irq_of_parse_and_map(node, 0);
    if (!irq) {
        printk("Failed to get INTC IRQ at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    }

    err = of_address_to_resource(node, 0, &res);
    if (err) {
        printk("Failed to get INTC memory range at %s line:%d err:%d\n", __func__, __LINE__, err);
        return err;
    }
    printk("%s line:%d res.start:0x%x\n", __func__, __LINE__, res.start);

    pres = request_mem_region(res.start, resource_size(&res), res.name);
    if (pres == NULL) {
        printk("Failed to request INTC memory at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    }

    /*
     * use [0+8, 63+8] as INTC virtual irq number, cpu has occupied 8 irq numbers
     * the intc domain is sencond domain, cpu-interrupt-controller as root domain
     */
    domain = irq_domain_add_legacy(node, INTC_INTERRUPT_NUM, 8, 0, &intc_irq_domain_ops, NULL);
    if (domain == NULL) {
        printk("Failed to add irq domain at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    }

    irq_set_chained_handler(irq, m200_intc_irq_handler);
    irq_set_handler_data(irq, domain);

    /*
     * Set default, after this we can use irq_find_mapping(NULL, hwirq) to get virtual irq
     */
    irq_set_default_host(domain);

    return 0;
}

/*
 * Not really use 'mti,cpu-interrupt-controller', we use vector table to deal with 8 cpu interrupts
 * in fact I'm not deal with cpu-interrupt-controller in irq subsystem, here used just for irq model
 * can't be deleted.
 * Note: irq [0, 7] will be first set by map function mips_cpu_intc_map() in arch/mips/kernel/irq_cpu.c
 *       then overwrited by another map function intc_map() in port/arch_init_irq.c
 */
static struct of_device_id __initdata of_irq_ids[] = {
    { .compatible = "mti,cpu-interrupt-controller", .data = mips_cpu_intc_init },
    { .compatible = "ingenic,m200-intc",            .data = soc_intc_of_init },
    { .compatible = "ingenic,m200-gpio",            .data = gpio_interrupt_of_init },
    { },
};

void __init arch_init_irq(void)
{
	/*
	 * Initialize Software/Hardware interrupt handlers
     * CPU int part(really)
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
	} else {
        printk("CPU can't support VINT %s line:%d\n", __func__, __LINE__);
        printk("'mti,cpu-interrupt-controller' can't be overwrite by intc irq at %s line:%d\n", __func__, __LINE__);
        printk("will use plat_irq_dispatch() deal with irqs\n");
    }

    /*
     * irq device tree
     * CPU int part(logical)
     * SoC intc part
     */
    of_irq_init(of_irq_ids);
}
