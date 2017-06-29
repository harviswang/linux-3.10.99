/*
 * invoked in arch/mips/kernel/setup.c arch_mem_init()
 */

#include <linux/init.h> /* __init */
#include <linux/of_fdt.h> /* initial_boot_params */
#include <linux/printk.h>
#include <linux/bootmem.h> /* reserve_bootmem free_bootmem  */
#include <asm/io.h> /* virt_to_phys */

void __init device_tree_init(void)
{
    unsigned long base, size;
    void *fdt_copy;

    if (!initial_boot_params) {
        printk("Non initial_boot_params at %s\n", __func__);
        return;
    }

    base = virt_to_phys((void *)initial_boot_params);
    size = be32_to_cpu(initial_boot_params->totalsize);

    /* Before we do anything, lets reserve the dt blob */
    reserve_bootmem(base, size, BOOTMEM_DEFAULT);

    /*
     * The strings in the flattened tree are referenced directly by the
     * device tree, so copy the flattened device tree from init memory
     * to regular memory.
     */
    fdt_copy = alloc_bootmem(size);
    memcpy(fdt_copy, initial_boot_params, size);
    initial_boot_params = fdt_copy;

    unflatten_device_tree();

    /* Free the space reserved for the dt blob */
    free_bootmem(base, size);
}

