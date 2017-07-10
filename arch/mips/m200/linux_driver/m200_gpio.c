#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <asm/gpio.h> /* struct gpio_chip */

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
    return 0;
}

static int m200_gpio_get(struct gpio_chip *chip, unsigned offset)
{
    return 0;
}

static int m200_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
    return 0;
}

static int m200_gpio_set_debounce(struct gpio_chip *chip, unsigned offset, unsigned debounce)
{
    return 0;
}

static void m200_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
}

static int m200_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
    return 0;
}

static void m200_gpio_deb_show(struct seq_file *s, struct gpio_chip *chip)
{
}

static struct gpio_chip m200_gpio_chip = {
    .label = "'M200 GPIO'",
    .owner = THIS_MODULE,
    .request = m200_gpio_request,
    .free    = m200_gpio_free,
    .get_direction    = m200_gpio_get_direction,
    .direction_input  = m200_gpio_direction_input,
    .get   = m200_gpio_get,
    .direction_output = m200_gpio_direction_output,
    .set_debounce     = m200_gpio_set_debounce,
    .set   = m200_gpio_set,
    .to_irq = m200_gpio_to_irq,
    .dbg_show         = m200_gpio_deb_show,
//    .base  = ,
//    .ngpio = ,
//    .desc  = ,
};

static struct of_device_id m200_gpio_of_match_table[] = {
    { .compatible = "ingenic,m200-gpio" },
    {}
};
MODULE_DEVICE_TABLE(of, m200_gpio_of_match_table);

static int m200_gpio_probe(struct platform_device *pdev)
{
    const struct of_device_id *match;

    match = of_match_device(m200_gpio_of_match_table, &pdev->dev);
    if (match == NULL) {
        dev_err(&pdev->dev, "Error: No device match found\n");
        return -ENODEV;
    }
    dev_info(&pdev->dev, "compatible:%s\n", match->compatible);
    gpiochip_add(&m200_gpio_chip);

    return 0;
}

static struct platform_driver m200_gpio_driver = {
    .driver = {
        .name = "'M200 GPIO'",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(m200_gpio_of_match_table),
    },

    .probe = m200_gpio_probe,
};

static int __init m200_gpio_init(void)
{
    return platform_driver_register(&m200_gpio_driver);
}

static void __exit m200_gpio_exit(void)
{
    platform_driver_unregister(&m200_gpio_driver);
}

postcore_initcall(m200_gpio_init);
module_exit(m200_gpio_exit);
