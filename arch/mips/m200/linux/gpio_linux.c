/*
 * GPIO Controller
 */

#include <linux/irq.h> /* struct irq_chip */
#include <linux/gpio.h> /* gpio_get_value */
#include <asm/irq.h> /* do_IRQ */
#include <asm/bitops.h> /* ffs */
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"
#include "../driverlib/gpio.h"

static void gpio_irq_enable(struct irq_data *data)
{
    unsigned long ulBase = (unsigned long)data->domain->host_data;
    unsigned long hwirq = data->hwirq;
    unsigned long ulPinNumber = hwirq % 32;

    GPIOInterruptEnable(ulBase, ulPinNumber);
}

/*
 * gpio hw irq start from 64, hw irq = 64 + 32*index + offset
 * index = GPIOPX - GPIOPA
 */
static void gpio_irq_disable(struct irq_data *data)
{
    unsigned long ulBase = (unsigned long)data->domain->host_data;
    unsigned long hwirq = data->hwirq;
    unsigned long ulPinNumber = hwirq % 32;

    GPIOInterruptDisable(ulBase, ulPinNumber);
}

static void gpio_irq_ack(struct irq_data *data)
{
    unsigned long ulBase = (unsigned long)data->domain->host_data;
    unsigned long hwirq = data->hwirq;
    unsigned long ulPinNumber = hwirq % 32;

    GPIOInterruptClear(ulBase, ulPinNumber);
}

static int gpio_irq_set_type(struct irq_data *data, unsigned int type)
{
    unsigned long ulBase = (unsigned long)data->domain->host_data;
    unsigned long hwirq = data->hwirq;
    unsigned long ulPinNumber = hwirq % 32;

    /* irq subsystem level */
    irqd_set_trigger_type(data, type);

    if (type & (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH)) {
        irq_set_handler(data->irq, handle_level_irq);
    } else if (type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING)) {
        irq_set_handler(data->irq, handle_edge_irq);
    } else {
        printk("Error: not support irq type:%d at:%s\n", type, __func__);
        return -EINVAL;
    }

    /*
     * SoC gpio level
     * Note: M200 SoC GPIO not really support both edge type interrupt
     * if you really need both edege triggered interrupt, you can use two
     * gpio irq, one using rasing edge, another using falling edge
     */
    switch (type) {
    case IRQ_TYPE_EDGE_BOTH:
        if (gpio_get_value(GPIONumberBaseGet(ulBase) + ulPinNumber)) {
            GPIOInterruptTypeSet(ulBase, ulPinNumber, GPIO_FUNCTION_FALLINT);
        } else {
            GPIOInterruptTypeSet(ulBase, ulPinNumber, GPIO_FUNCTION_RAISEINT);
        }
        break;
    case IRQ_TYPE_EDGE_FALLING:
        GPIOInterruptTypeSet(ulBase, ulPinNumber, GPIO_FUNCTION_FALLINT);
        break;
    case IRQ_TYPE_EDGE_RISING:
        GPIOInterruptTypeSet(ulBase, ulPinNumber, GPIO_FUNCTION_RAISEINT);
        break;
    case IRQ_TYPE_LEVEL_LOW:
        break;
    case IRQ_TYPE_LEVEL_HIGH:
        break; 
    default:
        printk("Unsupport irq type:%d at %s\n", type, __func__);
        break;
    }

    return IRQ_SET_MASK_OK;
}

static struct irq_chip gpio_irq_chip = {
    .name = "'GPIO Interrupt Controller'",
    .irq_mask    = gpio_irq_disable,
    .irq_unmask  = gpio_irq_enable,
    .irq_ack     = gpio_irq_ack,
    .irq_enable  = gpio_irq_enable,
    .irq_disable = gpio_irq_disable,
    .irq_set_type = gpio_irq_set_type,
    .flags       = IRQCHIP_SET_TYPE_MASKED,
};

static void gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
    struct irq_domain *domain = (struct irq_domain *)desc->irq_data.handler_data;
    unsigned long gpio_base = (unsigned long)domain->host_data;
    unsigned long gpio_int = HWREG(gpio_base + GPIO_O_INT);
    int i, bit_n;

    //printk("%s line:%d gpio_base:0x%x\n", __func__, __LINE__, (int)gpio_base);
    for (i = 31; i >= 0; i--) {
        if (gpio_int != 0) {
            bit_n = ffs(gpio_int);
            if (bit_n != 0) {
                bit_n -= 1;
                do_IRQ(bit_n + 8 + 64);
                gpio_int &= ~(1 << bit_n);
            } else {
                break;
            }
        }
    }
}

/*
 * only do irq_set_chip(), no handler set.
 * handler is set in gpio_irq_chip.irq_set_type()
 */
static int gpio_irq_domain_ops_map(struct irq_domain *id, unsigned int irq, irq_hw_number_t hw)
{
    (void)hw;
    irq_set_chip(irq, &gpio_irq_chip);

    return 0;
}

static const struct irq_domain_ops gpio_irq_domain_ops = {
    .xlate = irq_domain_xlate_twocell,
    .map   = gpio_irq_domain_ops_map,
};

static int
__init gpio_interrupt_of_init(struct device_node *node, struct device_node *parent)
{
    struct resource res;
    struct resource *pres;
    struct irq_domain *gpio_irq_domain;
    int irq;
    int err;
    int id;

    irq = irq_of_parse_and_map(node, 0);
    if (irq == 0) {
        printk("Failed to get GPIO(%s) irq at %s line:%d\n", node->name, __func__, __LINE__);
        return -EINVAL;
    }

    err = of_address_to_resource(node, 0, &res);
    if (err) {
        printk("Failed to get gpio(%s) memory range at %s line:%d err:%d\n", node->name, __func__, __LINE__, err);
        return err;
    }
    //printk("%s line:%d res.start:0x%x\n", __func__, __LINE__, res.start);

    pres = request_mem_region(res.start, resource_size(&res), res.name);
    if (pres == NULL) {
        printk("Faild to request %s memory at %s line:%d\n", node->name, __func__, __LINE__);
        return -EINVAL;
    }

    id = of_alias_get_id(node, "gpio");
    if (id < 0) {
        printk("Failed to get gpio id, error:%d at:%s\n", id, __func__);
        return id;
    }
    //printk("%s line:%d id:%d node:0x%p\n", __func__, __LINE__, id, node);

    /*
     * use res.start as domain's host_data, used in gpio_irq_handler/gpio_irq_enable
     * in order to get gpio base address
     */
    gpio_irq_domain = irq_domain_add_legacy(node, 32, 8 + 64 + id*32, 64 + id*32, &gpio_irq_domain_ops, (void *)res.start);
    if (gpio_irq_domain == NULL) {
        printk("Failed to add gpio irq domain at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    }

    irq_set_chained_handler(irq, gpio_irq_handler);
    irq_set_handler_data(irq, gpio_irq_domain);
    /*
     * I alse store res.start in node->data as private usage in m200_gpio.c
     */
    node->data = (void *)res.start;

    return 0;
}

