#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/pinctrl/pinconf.h>

#include "../driverlib/gpio.h" /* GPIONumberBaseGet */

/* drivers/pinctrl/core.h */
extern const char *pin_get_name(struct pinctrl_dev *pctldev, const unsigned pin);

struct m200_pinctrl_pin_configuration {
    unsigned char pin_function;
    unsigned char pin_pull; /* internal pull up/down resistance */
};

struct m200_pinctrl_pin {
    struct device_node *pin_gpio_node; /* which gpio port */
    unsigned int pin_offset; /* [0, 31] */
    struct m200_pinctrl_pin_configuration pin_default_configuration;
    struct m200_pinctrl_pin_configuration pin_sleep_configuration;
};

struct m200_pinctrl_group {
    const char *group_name;
    struct m200_pinctrl_pin *pins_conf;
    unsigned int *pins; /* store every pin offset, it's value [0, 31, 32, 63, ..] */
    int pin_count;
};

/*
 * function is the parent of group, it's a logic unit
 * for example function uart4 contains three groups
 * just use name to lookup
 */
struct m200_pinmux_function {
    const char  *function_name;
    const char  **group_names;
    int group_name_count;
};

struct m200_pinctrl {
    struct device *dev;
    struct pinctrl_dev *pinctrl_dev;
    struct m200_pinctrl_group *groups;
    int group_count;
    struct m200_pinmux_function *functions;
    int function_count;
};

/*
 * internal used
 */
static struct m200_pinctrl_group * m200_pinctrl_find_group_by_name(const struct m200_pinctrl *pinctrl, const char *name)
{
    int i;

    for (i = 0; i < pinctrl->group_count; i++) {
        if (strcmp((pinctrl->groups + i)->group_name, name) == 0) {
            return (pinctrl->groups + i);
        }
    }

    return NULL;
}

static int m200_pinctl_ops_get_groups_count(struct pinctrl_dev *pctldev)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    return pinctrl->group_count;
}

static const char* m200_pinctl_ops_get_group_name(struct pinctrl_dev *pctldev, unsigned selector)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    if (selector >= pinctrl->group_count) {
        dev_err(pinctrl->dev, "Invalid parameter 'selsector' at %s line:%d\n", __func__, __LINE__);
        return NULL;
    } else {
        return pinctrl->groups[selector].group_name;
    }
}

static int m200_pinctl_ops_get_group_pins(struct pinctrl_dev *pctldev, unsigned selector,
                   const unsigned **pins, unsigned *num_pins)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    if (selector >= pinctrl->group_count) {
        dev_err(pinctrl->dev, "Invalid parameter 'selsector' at %s line:%d\n", __func__, __LINE__);
        return -EINVAL;
    } else {
        *pins = (const unsigned *)pinctrl->groups[selector].pins; /* Note: I use a non int * pointer here */
        *num_pins = pinctrl->groups[selector].pin_count;
        return 0;
    }
}

static void m200_pinctl_ops_pin_dbg_show(struct pinctrl_dev *pctldev, struct seq_file *s, unsigned offset)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    seq_printf(s, "%s", dev_name(pinctrl->dev));
}

static int m200_pinctl_ops_dt_node_to_map(struct pinctrl_dev *pctldev, struct device_node *np,
                               struct pinctrl_map **map, unsigned *num_maps)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    struct m200_pinctrl_group *group;
    struct pinctrl_map *new_map;
    struct device_node *parent;
    int map_num = 1;
    int i;

    /*
     * first find the group of this node and check if we need create
     * config maps for pins
     */
    group = m200_pinctrl_find_group_by_name(pinctrl, np->name);
    if (group == NULL) {
        dev_err(pinctrl->dev, "unable to find group for node %s\n", np->name);
        return -EINVAL;
    }

    map_num += group->pin_count; /* device node map to a group and inner pins */
    new_map = devm_kzalloc(pinctrl->dev, sizeof(*new_map) * map_num, GFP_KERNEL);
    if (new_map == NULL) {
        dev_err(pinctrl->dev, "devm_kzalloc at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    *map = new_map;
    *num_maps = map_num;

    /* create mux map */
    parent = of_get_parent(np);
    if (parent == NULL) {
        devm_kfree(pinctrl->dev, new_map);
        return -EINVAL;
    }
    new_map[0].type = PIN_MAP_TYPE_MUX_GROUP;
    new_map[0].data.mux.function = parent->name;
    new_map[0].data.mux.group = np->name;
    of_node_put(parent);

    /* create config map */
    new_map++;
    for (i = 0; i < group->pin_count; i++) {
        new_map[i].type = PIN_MAP_TYPE_CONFIGS_PIN;
        new_map[i].data.configs.group_or_pin = pin_get_name(pctldev, group->pins[i]);
        //printk("%s line:%d group_or_pin:%s\n", __func__, __LINE__, new_map[i].data.configs.group_or_pin);
        new_map[i].data.configs.configs = (unsigned long *)&group->pins_conf[i];
        new_map[i].data.configs.num_configs = 1;
    }

    return 0;
}

static void m200_pinctl_ops_dt_free_map(struct pinctrl_dev *pctldev, struct pinctrl_map *map, unsigned num_maps)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    dev_err(pinctrl->dev, "%s line:%d\n", __func__, __LINE__);
    devm_kfree(pinctrl->dev, map);
}

static struct pinctrl_ops m200_pinctl_ops = {
    .get_groups_count = m200_pinctl_ops_get_groups_count,
    .get_group_name   = m200_pinctl_ops_get_group_name,
    .get_group_pins   = m200_pinctl_ops_get_group_pins,
    .pin_dbg_show     = m200_pinctl_ops_pin_dbg_show,
    .dt_node_to_map   = m200_pinctl_ops_dt_node_to_map,
    .dt_free_map      = m200_pinctl_ops_dt_free_map,
};

static int m200_pinmux_ops_request(struct pinctrl_dev *pctldev, unsigned offset)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_pinmux_ops_free(struct pinctrl_dev *pctldev, unsigned offset)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_pinmux_ops_get_functions_count(struct pinctrl_dev *pctldev)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    return pinctrl->function_count;
}

static const char *m200_pinmux_ops_get_function_name(struct pinctrl_dev *pctldev, unsigned selector)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    return pinctrl->functions[selector].function_name;
}

static int m200_pinmux_ops_get_function_groups(struct pinctrl_dev *pctldev, unsigned selector,
                                               const char * const **groups, unsigned * const num_groups)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    struct m200_pinmux_function *function = &pinctrl->functions[selector];

    *groups = function->group_names;
    *num_groups = function->group_name_count;

    return 0;
}

static int m200_pinmux_ops_enable(struct pinctrl_dev *pctldev, unsigned func_selector, unsigned group_selector)
{
    struct m200_pinctrl *pinctrl = pinctrl_dev_get_drvdata(pctldev);
    struct m200_pinctrl_group *group = &pinctrl->groups[group_selector];
    struct m200_pinmux_function *function = &pinctrl->functions[func_selector];
    int i;

    dev_dbg(pinctrl->dev, "enable function %s group %s\n", function->function_name, group->group_name);

    for (i = 0; i < group->pin_count; i++) {
        struct m200_pinctrl_pin *pin = &group->pins_conf[i];
        unsigned long gpio_mem_base = (unsigned long)pin->pin_gpio_node->data; /* prepared in gpio_linux.c */

        GPIOFunctionSet(gpio_mem_base, pin->pin_offset, pin->pin_default_configuration.pin_function);
        if (pin->pin_default_configuration.pin_pull) {
            GPIOPullEnable(gpio_mem_base, pin->pin_offset);
        } else {
            GPIOPullDisable(gpio_mem_base, pin->pin_offset);
        }
    }
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static struct pinmux_ops m200_pinmux_ops = {
    .request = m200_pinmux_ops_request,
    .free    = m200_pinmux_ops_free,
    .get_functions_count = m200_pinmux_ops_get_functions_count,
    .get_function_name   = m200_pinmux_ops_get_function_name,
    .get_function_groups = m200_pinmux_ops_get_function_groups,
    .enable = m200_pinmux_ops_enable,
};

static int m200_pinconf_ops_pin_config_get(struct pinctrl_dev *pctldev, unsigned pin, unsigned long *config)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_pinconf_ops_pin_config_set(struct pinctrl_dev *pctldev, unsigned pin, unsigned long config)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_pinconf_ops_pin_config_group_get(struct pinctrl_dev *pctldev, unsigned selector, unsigned long *config)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static int m200_pinconf_ops_pin_config_group_set(struct pinctrl_dev *pctldev, unsigned selector, unsigned long config)
{
    printk("%s line:%d\n", __func__, __LINE__);
    return 0;
}

static struct pinconf_ops m200_pinconf_ops = {
    .pin_config_get = m200_pinconf_ops_pin_config_get,
    .pin_config_set = m200_pinconf_ops_pin_config_set,
    .pin_config_group_get = m200_pinconf_ops_pin_config_group_get,
    .pin_config_group_set = m200_pinconf_ops_pin_config_group_set,
};

static struct pinctrl_desc m200_pinctrl_desc = {
    .pctlops = &m200_pinctl_ops,
    .pmxops  = &m200_pinmux_ops,
    .confops = &m200_pinconf_ops,
    .owner   = THIS_MODULE,
};

/*
 * parse group from dt
 * get pin and it's configuration
 * @group_index, the group index in a function, used for debug
 */
static int m200_pinctrl_group_parse_dt(struct device_node *group_node, struct m200_pinctrl_group *pinctrl_group,
                                       struct m200_pinctrl *pinctrl, int group_index)
{
    struct device *dev = pinctrl->dev;
    const __be32 *list;
    int size;
    int i;

    dev_dbg(dev, "pin group:%s at %s line:%d\n", group_node->name, __func__, __LINE__);

    /* Get group's name */
    pinctrl_group->group_name = group_node->name;

    /*
     * the binding format is ingenic,pins = <&gpioPX pin mux pll_config sleep_config>,
     * do sanity check and calculate pins number
     */
    list = of_get_property(group_node, "ingenic,pins", &size);
    size /= sizeof(*list); /* how many items in 'ingenic,pins' total */

    pinctrl_group->pin_count = size / 6; /* 6 is the item number in 'ingenic,pins' property for one pin */
    pinctrl_group->pins_conf = devm_kzalloc(dev, sizeof(struct m200_pinctrl_pin) * pinctrl_group->pin_count, GFP_KERNEL);
    if (pinctrl_group->pins_conf == NULL) {
        dev_err(dev, "devm_kzalloc failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    pinctrl_group->pins = devm_kzalloc(dev, sizeof(unsigned int) * pinctrl_group->pin_count, GFP_KERNEL);
    if (pinctrl_group->pins == NULL) {
        dev_err(dev, "devm_kzalloc failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    for (i = 0; i < pinctrl_group->pin_count; i++) {
        struct of_phandle_args pinspec;
        int err;
        unsigned long gpio_mem_base;

        err = of_parse_phandle_with_args(group_node, "ingenic,pins", "#pinctrl-cells", i, &pinspec);
        if (err != 0) {
            dev_err(dev, "%s line:%d err:%d node name:%s\n", __func__, __LINE__, err, pinspec.np->name);
            return -EINVAL;
        }

        pinctrl_group->pins_conf[i].pin_gpio_node = pinspec.np;
        pinctrl_group->pins_conf[i].pin_offset    = pinspec.args[0];
        pinctrl_group->pins_conf[i].pin_default_configuration.pin_function = pinspec.args[1];
        pinctrl_group->pins_conf[i].pin_default_configuration.pin_pull     = pinspec.args[2];
        pinctrl_group->pins_conf[i].pin_sleep_configuration.pin_function = pinspec.args[3];
        pinctrl_group->pins_conf[i].pin_sleep_configuration.pin_pull     = pinspec.args[4];

        gpio_mem_base = (unsigned long)pinctrl_group->pins_conf[i].pin_gpio_node->data; /* prepared in gpio_linux.c */
        pinctrl_group->pins[i] = GPIONumberBaseGet(gpio_mem_base) + pinctrl_group->pins_conf[i].pin_offset;
    }

    return 0;
}

/*
 * @param function_node, device_node for a function(such as uart3 function)
 * @param pinctrl, pinctrl pointer
 * @param function_index, function index
 * @param global_group_index, a pointer to global group index
 * return 0 on success, < 0 others
 */
static int m200_pinctrl_function_parse_dt(struct device_node *function_node, struct m200_pinctrl *pinctrl,
                                          int function_index, int *global_group_index)
{
    struct m200_pinmux_function *function;
    struct device_node *group_node;
    struct m200_pinctrl_group *group;
    struct device *dev = pinctrl->dev;
    int err = 0;
    int function_group_index;

    function = &pinctrl->functions[function_index];
    function->function_name = function_node->name;
    function->group_name_count = of_get_child_count(function_node);
    if (function->group_name_count <= 0) {
        dev_err(dev, "function %s without group, at %s line:%d\n", function->function_name, __func__, __LINE__);
        return -EINVAL;
    }
    function->group_names = devm_kzalloc(dev, sizeof(char *) * function->group_name_count, GFP_KERNEL);
    if (function->group_names == NULL) {
        dev_err(dev, "devm_kzalloc failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    function_group_index = 0;
    for_each_child_of_node(function_node, group_node) {
        function->group_names[function_group_index] = group_node->name;
        group = &pinctrl->groups[*global_group_index];
        err = m200_pinctrl_group_parse_dt(group_node, group, pinctrl, function_group_index);
        if (err) {
            dev_err(dev, "parse group in a function failed at %s line:%d\n", __func__, __LINE__);
            return -EINVAL;
        } else {
            function_group_index += 1;
            *global_group_index += 1;
        }
    }

    return 0;
}

static int m200_pinctrl_parse_dt(struct device_node *device_node, struct m200_pinctrl *pinctrl)
{
    struct device *dev = pinctrl->dev;
    struct device_node *function_node;
    int group_count = 0;
    int err = 0;
    int function_index;
    int group_index;

    /*
     * Iterate over all the child nodes of the pin controller node
     * and create pin groups and pin lists.
     */
    for_each_child_of_node(device_node, function_node) {
        group_count += of_get_child_count(function_node);
    }

    pinctrl->group_count = group_count;
    pinctrl->groups = kzalloc(sizeof(struct m200_pinctrl_group) * pinctrl->group_count, GFP_KERNEL);
    if (pinctrl->groups == NULL) {
        dev_err(dev, "%s line:%d kzalloc failed\n", __func__, __LINE__);
        return -ENOMEM;
    }

    pinctrl->function_count = of_get_child_count(device_node);
    pinctrl->functions = kzalloc(sizeof(struct m200_pinmux_function) * pinctrl->function_count, GFP_KERNEL);
    if (pinctrl->functions == NULL) {
        dev_err(dev, "%s line:%d kzalloc failed\n", __func__, __LINE__);
        return -ENOMEM;
    }

    function_index = 0;
    group_index = 0;
    for_each_child_of_node(device_node, function_node) {
        err = m200_pinctrl_function_parse_dt(function_node, pinctrl, function_index, &group_index);
        if (err) {
            dev_err(dev, "failed to parse pinctrl function\n");
            return err;
        } else {
            function_index++;
        }
   }

    return 0;
}

static int m200_pinctrl_probe(struct platform_device *pdev)
{
    struct m200_pinctrl *pinctrl;
    struct device *dev = &pdev->dev;
    struct device_node *device_node = dev->of_node;
    int i, j, k;

    pinctrl = devm_kzalloc(dev, sizeof(*pinctrl), GFP_KERNEL);
    if (pinctrl == NULL) {
        dev_err(dev, "devm_kzalloc failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    pinctrl->dev = dev;
    m200_pinctrl_parse_dt(device_node, pinctrl);

    m200_pinctrl_desc.name = dev_name(dev);
    m200_pinctrl_desc.npins = 0;
    for(i = 0; i < pinctrl->group_count; i++) {
        struct m200_pinctrl_group *group = &pinctrl->groups[i];
        m200_pinctrl_desc.npins += group->pin_count;
    }

    m200_pinctrl_desc.pins = devm_kzalloc(dev, sizeof(struct pinctrl_pin_desc) * m200_pinctrl_desc.npins, GFP_KERNEL);
    if (m200_pinctrl_desc.pins == NULL) {
        dev_err(dev, "devm_kzalloc failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }
    for(i = 0, k = 0; i < pinctrl->group_count; i++) {
        struct m200_pinctrl_group *group = &pinctrl->groups[i];
        for (j = 0; j < group->pin_count; j++) {
            struct pinctrl_pin_desc *pin_desc = (struct pinctrl_pin_desc *)m200_pinctrl_desc.pins; /* writeable */
            pin_desc[k].number = group->pins[j];
            pin_desc[k].name   = kasprintf(GFP_KERNEL, "%s:%s.%d", group->group_name,
                                                group->pins_conf[j].pin_gpio_node->name, group->pins_conf[j].pin_offset);
            k++;
        }
    }

    pinctrl->pinctrl_dev = pinctrl_register(&m200_pinctrl_desc, dev, pinctrl);
    if (pinctrl->pinctrl_dev == NULL) {
        dev_err(dev, "pinctrl_register failed at %s line:%d\n", __func__, __LINE__);
        return -ENOMEM;
    }

    platform_set_drvdata(pdev, pinctrl);

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
