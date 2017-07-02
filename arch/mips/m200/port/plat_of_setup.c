/*
 * populate platform device
 */

#include <linux/init.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>


int __init plat_of_setup(void)
{
    static struct of_device_id of_ids[3];

    memset(of_ids, 0x0, sizeof(of_ids));
    if (of_have_populated_dt()) {
        strncpy(of_ids[0].compatible, "ingenic,m200", sizeof(of_ids[0].compatible));
        strncpy(of_ids[1].compatible, "simple-bus", sizeof(of_ids[1].compatible));
        return of_platform_populate(NULL, of_ids, NULL, NULL);
    } else {
        printk("Error: at %s line:%d\n", __func__, __LINE__);
        panic("device tree not present");
    }
}

arch_initcall(plat_of_setup);
