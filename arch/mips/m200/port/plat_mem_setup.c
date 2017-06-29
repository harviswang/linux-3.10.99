#include <linux/ioport.h>
#include <linux/of_fdt.h> /* boot_param_header */

#include <asm/io.h>
#include <asm/prom.h> /* __dt_setup_arch */

/*
 * __dtb_start defined in arch/mips/kernel/vmlinux.lds
 * mips device tree is built-in
 */
extern struct boot_param_header __dtb_start;

void __init plat_mem_setup(void)
{
	/* ingenic mips cpu special */
	__asm__ (
		"li    $2, 0xa9000000 \n\t"
		"mtc0  $2, $5, 4      \n\t"
		"nop                  \n\t"
		::"r"(2)); /*  TODO */

	/*
	 * IO memory space
	 */
	set_io_port_base(IO_BASE);

	/*
	 *
	 */
	ioport_resource.start = 0x00000000;
	ioport_resource.end   = 0xffffffff;
	iomem_resource.start = 0x00000000;
	iomem_resource.end   = 0xffffffff;

    /*
     * Load the builtin devicetree. This causes the chosen node to be
     * parsed resulting in our memory appearing
     */
    __dt_setup_arch(&__dtb_start);

}

