#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* container_of */
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h> /* of_gpiochip_add */
#include <linux/gpio.h>
#include <linux/slab.h> /* kmalloc */
#include <asm/gpio.h> /* struct gpio_chip */

#include "../driverlib/gpio.h" /* GPIOBaseGet/GPIOPinLevelGet */

struct m200_gpio_chip {
    struct gpio_chip gpio_chip;
    unsigned long gpio_mem_base; /* used to store the memory base address of a gpio port
                                  */
};

static int m200_gpio_request(struct gpio_chip *chip, unsigned offset)
{
    return 0;
}

static void m200_gpio_free(struct gpio_chip *chip, unsigned offset)
{
}

static int m200_gpio_get_direction(struct gpio_chip *chip, unsigned offset)
{
    return 0;
}

static int m200_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
    unsigned long ulBase = ((struct m200_gpio_chip *)chip)->gpio_mem_base;
    int result = GPIOFunctionSet(ulBase, offset, GPIO_FUNCTION_INTPUT);
    return result == 1;
}

static int m200_gpio_get(struct gpio_chip *chip, unsigned offset)
{
    unsigned long ulBase = ((struct m200_gpio_chip *)chip)->gpio_mem_base;
    return GPIOPinLevelGet(ulBase, offset);
}

static int m200_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
    unsigned long ulBase = ((struct m200_gpio_chip *)chip)->gpio_mem_base;

    if (value) {
        GPIOFunctionSet(ulBase, offset, GPIO_FUNCTION_OUTPUT1);
    } else {
        GPIOFunctionSet(ulBase, offset, GPIO_FUNCTION_OUTPUT0);
    }

    return 0;
}

static int m200_gpio_set_debounce(struct gpio_chip *chip, unsigned offset, unsigned debounce)
{
    return 0;
}

static void m200_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
    m200_gpio_direction_output(chip, offset, value);
}

static int m200_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
    return 8 + 64 + chip->base + offset;
}

static void m200_gpio_deb_show(struct seq_file *s, struct gpio_chip *chip)
{
}

static struct of_device_id m200_gpio_of_match_table[] = {
    { .compatible = "ingenic,m200-gpio" },
    {}
};
MODULE_DEVICE_TABLE(of, m200_gpio_of_match_table);

static int m200_gpio_probe(struct platform_device *pdev)
{
    const struct of_device_id *match;
    struct m200_gpio_chip *mgc;
    struct device_node *device_node = pdev->dev.of_node;
    int err;
    u32 ngpios;

    match = of_match_device(m200_gpio_of_match_table, &pdev->dev);
    if (match == NULL) {
        dev_err(&pdev->dev, "Error: No device match found\n");
        return -ENODEV;
    }
    dev_info(&pdev->dev, "compatible:%s\n", match->compatible);

    mgc = kmalloc(sizeof(struct m200_gpio_chip), GFP_KERNEL);
    if (mgc == NULL) {
        dev_err(&pdev->dev, "Error: No memory malloc at:%s\n", __func__);
        return -ENOMEM;
    }

    memset(mgc, 0x0, sizeof(struct m200_gpio_chip));
    mgc->gpio_chip.owner   = THIS_MODULE;
    mgc->gpio_chip.dev     = &pdev->dev;
    mgc->gpio_chip.request = m200_gpio_request;
    mgc->gpio_chip.free    = m200_gpio_free;
    mgc->gpio_chip.get_direction   = m200_gpio_get_direction;
    mgc->gpio_chip.direction_input = m200_gpio_direction_input;
    mgc->gpio_chip.get     = m200_gpio_get;
    mgc->gpio_chip.direction_output = m200_gpio_direction_output;
    mgc->gpio_chip.set_debounce     = m200_gpio_set_debounce;
    mgc->gpio_chip.set     = m200_gpio_set;
    mgc->gpio_chip.to_irq  = m200_gpio_to_irq;
    mgc->gpio_chip.dbg_show         = m200_gpio_deb_show;
    mgc->gpio_mem_base = (unsigned long)device_node->data;/* prepared in gpio_linux.c */
    mgc->gpio_chip.base  = GPIONumberBaseGet(mgc->gpio_mem_base);
    err = of_property_read_u32_index(device_node, "ngpios", 0, &ngpios);
    if (err) {
        dev_err(&pdev->dev, "Error: no ngpios property defined\n");
        return err;
    }
    mgc->gpio_chip.ngpio = ngpios;
    //mgc->gpio_chip.of_node = pdev->dev.of_node;
    //mgc->gpio_chip.of_gpio_n_cells = 2;
    //mgc->gpio_chip.of_xlate = of_gpio_simple_xlate;
    of_gpiochip_add(&mgc->gpio_chip);

    printk("%s line:%d base:%d ngpio:%d mgc->gpio_mem_base:0x%x\n", __func__, __LINE__, mgc->gpio_chip.base, mgc->gpio_chip.ngpio, (int)mgc->gpio_mem_base);
    gpiochip_add(&mgc->gpio_chip);

    platform_set_drvdata(pdev, mgc);

    return 0;
}

static int m200_gpio_remove(struct platform_device *pdev)
{
    int err;
    struct m200_gpio_chip *mgc = platform_get_drvdata(pdev);

    printk("%s line:%d\n", __func__, __LINE__);
    of_gpiochip_remove(&mgc->gpio_chip);
    err = gpiochip_remove(&mgc->gpio_chip);
    if (err) {
        dev_err(&pdev->dev, "Error: remove a gpiochip failed at:%s\n", __func__);
    }

    kfree(mgc);
    mgc = NULL;

    return err;
}

static struct platform_driver m200_gpio_driver = {
    .driver = {
        .name = "m200_gpio",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(m200_gpio_of_match_table),
    },

    .probe = m200_gpio_probe,
    .remove= m200_gpio_remove,
};
module_platform_driver(m200_gpio_driver);

MODULE_DESCRIPTION("M200 GPIO Driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang");
