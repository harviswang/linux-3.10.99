/*
 * emulate a soft interrupt, check interrupt handler works or not 
 * VINT(set_vi_handler) works or not
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

int __init cpu_interrupt_test(void)
{
    //set_c0_status(1);
    set_c0_status(1 << 8 | 1 << 9 | 1 << 10);
    set_c0_cause(1 << 8 |   1 << 10);
    printk(KERN_ERR"read_c0_cause() = 0x%x\n", read_c0_cause());
    printk(KERN_ERR"read_c0_status() = 0x%x\n", read_c0_status());
    printk(KERN_ERR"read_c0_intctl() = 0x%x\n", read_c0_intctl());
    printk(KERN_ERR"read_c0_config3() = 0x%x\n", read_c0_config3());
    printk(KERN_ERR "read_c0_ebase() = 0x%x\n", read_c0_ebase());

    return 0;
}

module_init(cpu_interrupt_test);

