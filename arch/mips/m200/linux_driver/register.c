/*
 * Copyright (c) 2017 Harvis Wang.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Used to debug, see the value of some register */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>

static unsigned char bit_width = 32; /* can be 32/16/8 */
static unsigned int  last_register_address = 0xB0002000; /* store register address coming from register_address_store() 
                                                          * show register address to register_address_show()
                                                          */

static ssize_t register_address_show(struct device *dev, struct device_attribute *attr, 
    char *buf)
{
    ssize_t count;

    device_lock(dev);
    count = sprintf(buf, "0x%08X\n", last_register_address);
    device_unlock(dev);

    return count;
}

static ssize_t register_address_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    device_lock(dev);
    last_register_address = (unsigned int)simple_strtol(buf, NULL, 16);
    device_unlock(dev);

    return count;
}
DEVICE_ATTR(register_address, S_IRUGO | S_IWUSR | S_IWGRP, register_address_show, register_address_store);

static ssize_t register_value_show(struct device *dev, struct device_attribute *attr, 
    char *buf)
{
    unsigned int value;
    ssize_t count;

    device_lock(dev);
    switch (bit_width) {
    case 32:
        value = readl((void *)last_register_address);
        count = sprintf(buf, "0x%08X\n", value);
        break;
    case 16:
        value = readw((void *)last_register_address);
        count = sprintf(buf, "0x%04X\n", value);
        break;
    case 8:
        value = readb((void *)last_register_address);
        count = sprintf(buf, "%0x02X\n", value);
        break;
    default:
        count = sprintf(buf, "bit_width can only be 32/16/8\n");
        break;
    }
    device_unlock(dev);

    return count;
}

static ssize_t register_value_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    unsigned int value;

    device_lock(dev);
    value = simple_strtol(buf, NULL, 16);
    switch (bit_width) {
    case 32:
        writel(value, (void *)last_register_address);
        break;
    case 16:
        writew(value, (void *)last_register_address);
        break;
    case 8:
        writeb(value, (void *)last_register_address);
        break;
    default:
        dev_err(dev, "invalid bitwidth, can be 32/16/8\n");
        break;
    }
    device_unlock(dev);

    return count;
}
DEVICE_ATTR(register_value, S_IRUGO | S_IWUSR | S_IWGRP, register_value_show, register_value_store);

static ssize_t register_bitwidth_show(struct device *dev, struct device_attribute *attr, 
    char *buf)
{
    ssize_t count;

    device_lock(dev);
    count = sprintf(buf, "%d\n", bit_width);
    device_unlock(dev);

    return count;
}

static ssize_t register_bitwidth_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    unsigned int value;

    device_lock(dev);
    value = simple_strtol(buf, NULL, 10);

    switch (value) {
    case 8:
        bit_width = 8;
        break;
    case 16:
        bit_width = 16;
        break;
    case 32:
        bit_width = 32;
        break;
    default:
        bit_width = 32;
        break;
    }
    device_unlock(dev);

    return count;
}
DEVICE_ATTR(register_bitwidth, S_IRUGO | S_IWUSR | S_IWGRP, register_bitwidth_show, register_bitwidth_store);

static int register_probe(struct platform_device *pdev)
{
    int error = 0;

    error = device_create_file(&pdev->dev, &dev_attr_register_address);
    dev_info(&pdev->dev, "%s line:%d error = %d\n", __func__, __LINE__, error);

    error = device_create_file(&pdev->dev, &dev_attr_register_value);
    dev_info(&pdev->dev, "%s line:%d error = %d\n", __func__, __LINE__, error);

    error = device_create_file(&pdev->dev, &dev_attr_register_bitwidth);
    dev_info(&pdev->dev, "%s line:%d error = %d\n", __func__, __LINE__, error);

    return error;
}

static struct platform_driver register_driver = {
    .probe = register_probe,
    .driver = {
        .name = "register",
        .owner = THIS_MODULE,
    },
    
};

static struct platform_device register_device = {
    .name = "register",
    .id = 1,
    .num_resources = 0,
};

static int register_entry(void)
{
	int err = 0;

	err = platform_device_register(&register_device);
	err += platform_driver_register(&register_driver);

	return err;
}

static void register_exit(void)
{

}

arch_initcall(register_entry);
module_exit(register_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang");
