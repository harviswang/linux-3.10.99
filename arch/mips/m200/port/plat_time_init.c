/*
 * invoked in arch/mips/kernel/time.c:time_init()
 */

#include <linux/init.h>
#include "../linux/ost_linux.h"

void __init plat_time_init(void)
{
	ost_time_init();
}

