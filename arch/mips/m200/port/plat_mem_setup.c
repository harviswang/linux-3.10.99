#include <linux/ioport.h>

#include <asm/io.h>

int plat_mem_setup(void)
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

	return 0;
}

