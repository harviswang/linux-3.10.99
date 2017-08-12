#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/pinctrl/pinctrl.h>

static int m200_get_groups_count(struct pinctrl_dev *pctldev)
{
    return 0;
}

static const char* m200_get_group_name(struct pinctrl_dev *pctldev,
                       unsigned selector)
{
    return "pin";
}

static int m200_get_group_pins(struct pinctrl_dev *pctldev, unsigned selector,
                   const unsigned **pins, unsigned *num_pins)
{
    return 0;
}

static struct pinctrl_ops m200_pctlops = {
    .get_groups_count = m200_get_groups_count,
    .get_group_name   = m200_get_group_name,
    .get_group_pins   = m200_get_group_pins,
};

static struct pinctrl_desc m200_pinctrl_desc = {
    .name = "m200_pinctrl",
    .pctlops = &m200_pctlops,
//    .pmxops  = &m200_pmxops,
//    .confops = &m200_confops,
    .owner   = THIS_MODULE,
};

static int m200_pinctrl_group_count_parse_dt(struct device_node *parent_node)
{
    int group_count = 0;
    struct device_node *child_node;

    for_each_child_of_node(parent_node, child_node) {
        group_count += of_get_child_count(child_node);
    }

    return group_count;
}

static int m200_pinctrl_parse_dt(struct device_node *parent_node)
{
    const __be32 *list;
    int size;
    struct device_node *child_node;
    int group_count = m200_pinctrl_group_count_parse_dt(parent_node);

    /*
     * Iterate over all the child nodes of the pin controller node
     * and create pin groups and pin lists.
     */
    for_each_child_of_node(parent_node, child_node) {
        printk("%s line:%d node name:%s\n", __func__, __LINE__, child_node->name);
    }

    /*
     * the binding format is ingenic,pins = <&gpioPX pin mux pll_config sleep_config>,
     * do sanity check and calculate pins number
     */
    list = of_get_property(child_node, "ingenic,pins", &size); 
    size /= sizeof(*list);
    printk("%s line:%d group_count:%d size:%d\n", __func__, __LINE__, group_count, size);

    return 0;
}

static int m200_pinctrl_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct pinctrl_dev *pinctrl_dev;
    struct device_node *device_node = dev->of_node;

    pinctrl_dev = pinctrl_register(&m200_pinctrl_desc, dev, NULL);
    if (pinctrl_dev == NULL) {
        dev_err(dev, "Failed to alloc pinctrl_dev at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    m200_pinctrl_parse_dt(device_node); 
    platform_set_drvdata(pdev, pinctrl_dev);
    return 0;
}

static int m200_pinctrl_remove(struct platform_device *pdev)
{
    return 0;
}

static struct of_device_id m200_pinctrl_of_match_table[] = {
    { .compatible = "ingenic,m200-pinctrl" },
    {},
};

static struct platform_driver m200_pinctrl_driver = {
    .driver = {
        .name = "m200_pinctrl",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(m200_pinctrl_of_match_table),
    },

    .probe = m200_pinctrl_probe,
    .remove= m200_pinctrl_remove,
};
module_platform_driver(m200_pinctrl_driver);

MODULE_DESCRIPTION("M200 Pin Control Driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang");
